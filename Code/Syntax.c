#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
 
#define NELEMS(arr) (sizeof(arr) / sizeof(arr[0]))
 
typedef enum {
    tk_EOI, tk_Mul, tk_Div, tk_Mod, tk_Add, tk_Sub, tk_Negate, tk_Not, tk_Lss, tk_Leq, tk_Gtr,
    tk_Geq, tk_Eql, tk_Neq, tk_Assign, tk_And, tk_Or, tk_If, tk_Else, tk_While, tk_Print,
    tk_Putc, tk_Lparen, tk_Rparen, tk_Lbrace, tk_Rbrace, tk_Semi, tk_Comma, tk_Ident,
    tk_Integer, tk_String
} TokenType;
 
typedef enum {
    nd_Ident, nd_String, nd_Integer, nd_Sequence, nd_If, nd_Prtc, nd_Prts, nd_Prti, nd_While,
    nd_Assign, nd_Negate, nd_Not, nd_Mul, nd_Div, nd_Mod, nd_Add, nd_Sub, nd_Lss, nd_Leq,
    nd_Gtr, nd_Geq, nd_Eql, nd_Neq, nd_And, nd_Or
} NodeType;
 
typedef struct {
    TokenType token;
    int err_ln;
    int err_col;
    char *text;             /* ident or string literal or integer value */
} token_s;
 
typedef struct Tree {
    NodeType node_type;
    struct Tree *left;
    struct Tree *right;
    char *value;
} Tree;
 
// dependency: Ordered by token, must remain in same order as TokenType enum
struct {
    char       *text, *enum_text;
    TokenType   token;
    bool        right_associative, is_binary, is_unary;
    int         precedence;
    NodeType    node_type;
} atr[] = {
    {"EOI",             "End_of_input"   , tk_EOI,     false, false, false, -1, -1        },
    {"*",               "Op_multiply"    , tk_Mul,     false, true,  false, 13, nd_Mul    },
    {"/",               "Op_divide"      , tk_Div,     false, true,  false, 13, nd_Div    },
    {"%",               "Op_mod"         , tk_Mod,     false, true,  false, 13, nd_Mod    },
    {"+",               "Op_add"         , tk_Add,     false, true,  false, 12, nd_Add    },
    {"-",               "Op_subtract"    , tk_Sub,     false, true,  false, 12, nd_Sub    },
    {"-",               "Op_negate"      , tk_Negate,  false, false, true,  14, nd_Negate },
    {"!",               "Op_not"         , tk_Not,     false, false, true,  14, nd_Not    },
    {"<",               "Op_less"        , tk_Lss,     false, true,  false, 10, nd_Lss    },
    {"<=",              "Op_lessequal"   , tk_Leq,     false, true,  false, 10, nd_Leq    },
    {">",               "Op_greater"     , tk_Gtr,     false, true,  false, 10, nd_Gtr    },
    {">=",              "Op_greaterequal", tk_Geq,     false, true,  false, 10, nd_Geq    },
    {"==",              "Op_equal"       , tk_Eql,     false, true,  false,  9, nd_Eql    },
    {"!=",              "Op_notequal"    , tk_Neq,     false, true,  false,  9, nd_Neq    },
    {"=",               "Op_assign"      , tk_Assign,  false, false, false, -1, nd_Assign },
    {"&&",              "Op_and"         , tk_And,     false, true,  false,  5, nd_And    },
    {"||",              "Op_or"          , tk_Or,      false, true,  false,  4, nd_Or     },
    {"if",              "Keyword_if"     , tk_If,      false, false, false, -1, nd_If     },
    {"else",            "Keyword_else"   , tk_Else,    false, false, false, -1, -1        },
    {"while",           "Keyword_while"  , tk_While,   false, false, false, -1, nd_While  },
    {"print",           "Keyword_print"  , tk_Print,   false, false, false, -1, -1        },
    {"putc",            "Keyword_putc"   , tk_Putc,    false, false, false, -1, -1        },
    {"(",               "LeftParen"      , tk_Lparen,  false, false, false, -1, -1        },
    {")",               "RightParen"     , tk_Rparen,  false, false, false, -1, -1        },
    {"{",               "LeftBrace"      , tk_Lbrace,  false, false, false, -1, -1        },
    {"}",               "RightBrace"     , tk_Rbrace,  false, false, false, -1, -1        },
    {";",               "Semicolon"      , tk_Semi,    false, false, false, -1, -1        },
    {",",               "Comma"          , tk_Comma,   false, false, false, -1, -1        },
    {"Ident",           "Identifier"     , tk_Ident,   false, false, false, -1, nd_Ident  },
    {"Integer literal", "Integer"        , tk_Integer, false, false, false, -1, nd_Integer},
    {"String literal",  "String"         , tk_String,  false, false, false, -1, nd_String },
};
 
char *Display_nodes[] = {"Identifier", "String", "Integer", "Sequence", "If", "Prtc",
    "Prts", "Prti", "While", "Assign", "Negate", "Not", "Multiply", "Divide", "Mod",
    "Add", "Subtract", "Less", "LessEqual", "Greater", "GreaterEqual", "Equal",
    "NotEqual", "And", "Or"};
 
static token_s token;
static FILE *source_fp, *dest_fp;
 
Tree *paren_expr();
 
void error(int err_line, int err_col, const char *fmt, ... ) {
    va_list ap;
    char buf[1000];
 
    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);
    printf("(%d, %d) error: %s\n", err_line, err_col, buf);
    exit(1);
}
 
char *read_line(int *len) {
    static char *text = NULL;
    static int textmax = 0;
 
    for (*len = 0; ; (*len)++) {
        int ch = fgetc(source_fp);
        if (ch == EOF || ch == '\n') {
            if (*len == 0)
                return NULL;
            break;
        }
        if (*len + 1 >= textmax) {
            textmax = (textmax == 0 ? 128 : textmax * 2);
            text = realloc(text, textmax);
        }
        text[*len] = ch;
    }
    text[*len] = '\0';
    return text;
}
 
char *rtrim(char *text, int *len) {         // remove trailing spaces
    for (; *len > 0 && isspace(text[*len - 1]); --(*len))
        ;
 
    text[*len] = '\0';
    return text;
}
 
TokenType get_enum(const char *name) {      // return internal version of name
    for (size_t i = 0; i < NELEMS(atr); i++) {
        if (strcmp(atr[i].enum_text, name) == 0)
            return atr[i].token;
    }
    error(0, 0, "Unknown Token %s\n", name);
    return 0;
}
 
token_s gettoken() {
    int len;
    token_s token;
    char *yytext = read_line(&len);
    yytext = rtrim(yytext, &len);
 
    // [ ]*{lineno}[ ]+{colno}[ ]+Token[ ]+optional
 
    // get line and column
    token.err_ln  = atoi(strtok(yytext, " "));
    token.err_col = atoi(strtok(NULL, " "));
 
    // get the Token name
    char *name = strtok(NULL, " ");
    token.token = get_enum(name);
 
    // if there is extra data, get it
    char *p = name + strlen(name);
    if (p != &yytext[len]) {
        for (++p; isspace(*p); ++p)
            ;
        token.text = strdup(p);
    }
    return token;
}
 
Tree *make_node(NodeType node_type, Tree *left, Tree *right) {
    Tree *t = calloc(sizeof(Tree), 1);
    t->node_type = node_type;
    t->left = left;
    t->right = right;
    return t;
}
 
Tree *make_leaf(NodeType node_type, char *value) {
    Tree *t = calloc(sizeof(Tree), 1);
    t->node_type = node_type;
    t->value = strdup(value);
    return t;
}
 
void nexttoken(const char msg[], TokenType s) {
    if (token.token == s) {
        token = gettoken();
        return;
    }
    error(token.err_ln, token.err_col, "%s: Expecting '%s', found '%s'\n", msg, atr[s].text, atr[token.token].text);
}
 
Tree *expr(int p) {
    Tree *x = NULL, *node;
    TokenType op;
 
    switch (token.token) {
        case tk_Lparen:
            x = paren_expr();
            break;
        case tk_Sub: case tk_Add:
            op = token.token;
            token = gettoken();
            node = expr(atr[tk_Negate].precedence);
            x = (op == tk_Sub) ? make_node(nd_Negate, node, NULL) : node;
            break;
        case tk_Not:
            token = gettoken();
            x = make_node(nd_Not, expr(atr[tk_Not].precedence), NULL);
            break;
        case tk_Ident:
            x = make_leaf(nd_Ident, token.text);
            token = gettoken();
            break;
        case tk_Integer:
            x = make_leaf(nd_Integer, token.text);
            token = gettoken();
            break;
        default:
            error(token.err_ln, token.err_col, "expecting a primary, found: %s\n", atr[token.token].text);
    }
 
    while (atr[token.token].is_binary && atr[token.token].precedence >= p) {
        TokenType op = token.token;
 
        token = gettoken();
 
        int q = atr[op].precedence;
        if (!atr[op].right_associative)
            q++;
 
        node = expr(q);
        x = make_node(atr[op].node_type, x, node);
    }
    return x;
}
 
Tree *paren_expr() {
    nexttoken("paren_expr", tk_Lparen);
    Tree *t = expr(0);
    nexttoken("paren_expr", tk_Rparen);
    return t;
}
 
Tree *stmt() {
    Tree *t = NULL, *v, *e, *s, *s2;
 
    switch (token.token) {
        case tk_If:
            token = gettoken();
            e = paren_expr();
            s = stmt();
            s2 = NULL;
            if (token.token == tk_Else) {
                token = gettoken();
                s2 = stmt();
            }
            t = make_node(nd_If, e, make_node(nd_If, s, s2));
            break;
        case tk_Putc:
            token = gettoken();
            e = paren_expr();
            t = make_node(nd_Prtc, e, NULL);
            nexttoken("Putc", tk_Semi);
            break;
        case tk_Print: /* print '(' expr {',' expr} ')' */
            token = gettoken();
            for (nexttoken("Print", tk_Lparen); ; nexttoken("Print", tk_Comma)) {
                if (token.token == tk_String) {
                    e = make_node(nd_Prts, make_leaf(nd_String, token.text), NULL);
                    token = gettoken();
                } else
                    e = make_node(nd_Prti, expr(0), NULL);
 
                t = make_node(nd_Sequence, t, e);
 
                if (token.token != tk_Comma)
                    break;
            }
            nexttoken("Print", tk_Rparen);
            nexttoken("Print", tk_Semi);
            break;
        case tk_Semi:
            token = gettoken();
            break;
        case tk_Ident:
            v = make_leaf(nd_Ident, token.text);
            token = gettoken();
            nexttoken("assign", tk_Assign);
            e = expr(0);
            t = make_node(nd_Assign, v, e);
            nexttoken("assign", tk_Semi);
            break;
        case tk_While:
            token = gettoken();
            e = paren_expr();
            s = stmt();
            t = make_node(nd_While, e, s);
            break;
        case tk_Lbrace:         /* {stmt} */
            for (nexttoken("Lbrace", tk_Lbrace); token.token != tk_Rbrace && token.token != tk_EOI;)
                t = make_node(nd_Sequence, t, stmt());
            nexttoken("Lbrace", tk_Rbrace);
            break;
        case tk_EOI:
            break;
        default: error(token.err_ln, token.err_col, "expecting start of statement, found '%s'\n", atr[token.token].text);
    }
    return t;
}
 
Tree *parse() {
    Tree *t = NULL;
 
    token = gettoken();
    do {
        t = make_node(nd_Sequence, t, stmt());
    } while (t != NULL && token.token != tk_EOI);
    return t;
}
 
void prt_ast(Tree *t) {
    if (t == NULL)
        printf(";\n");
    else {
        printf("%-14s ", Display_nodes[t->node_type]);
        if (t->node_type == nd_Ident || t->node_type == nd_Integer || t->node_type == nd_String) {
            printf("%s\n", t->value);
        } else {
            printf("\n");
            prt_ast(t->left);
            prt_ast(t->right);
        }
    }
}
 
void init_io(FILE **fp, FILE *std, const char mode[], const char fn[]) {
    if (fn[0] == '\0')
        *fp = std;
    else if ((*fp = fopen(fn, mode)) == NULL)
        error(0, 0, "Can't open %s\n", fn);
}
 
int main(int argc, char *argv[]) {
    init_io(&source_fp, stdin,  "r",  argc > 1 ? argv[1] : "");
    init_io(&dest_fp,   stdout, "wb", argc > 2 ? argv[2] : "");
    prt_ast(parse());
}