
Backus Naur Form is a code that expresses grammar of formal languages(programming language)

   stmt_list           =   {stmt} ;    /* The start symbol */
 
    stmt                =   ';'
                          | Identifier '=' expr ';'
                          | 'while' paren_expr stmt
                          | 'if' paren_expr stmt ['else' stmt]
                          | 'print' '(' prt_list ')' ';'
                          | 'putc' paren_expr ';'
                          | '{' stmt_list '}'
                          ;
 
    paren_expr          =   '(' expr ')' ;
 
    prt_list            =   (string | expr) {',' (String | expr)} ;
 
    expr                =   and_expr            {'||' and_expr} ;
    and_expr            =   equality_expr       {'&&' equality_expr} ;
    equality_expr       =   relational_expr     [('==' | '!=') relational_expr] ;
    relational_expr     =   addition_expr       [('<' | '<=' | '>' | '>=') addition_expr] ;
    addition_expr       =   multiplication_expr {('+' | '-') multiplication_expr} ;
    multiplication_expr =   primary             {('*' | '/' | '%') primary } ;
    primary             =   Identifier
                          | Integer
                          | '(' expr ')'
                          | ('+' | '-' | '!') primary
                          ;