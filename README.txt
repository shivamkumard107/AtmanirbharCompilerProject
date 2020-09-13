<!--
*** Thanks for checking out this README. If you have a suggestion that would
*** make this better, please fork the repo and create a pull request or simply open
*** an issue with the tag "enhancement".
*** Thanks again! Now go create something AMAZING! :D
-->


<!-- PROJECT COMPILER -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/shivamkumard107/AtmanirbharCompiler/res/compiler-design.png">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>

  <h2 align="center">Atmanirbhar Compiler Project</h2>

  <p align="center">
    A basic compiler made using C language, using Extended Backus–Naur form(EBNF) for defining the grammar of the formal language. 
    <br />
    <a href="https://github.com/shivamkumard107/AtmanirbharCompiler/README.txt"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://youtube.com/">View Demo</a>
    ·
    <a href="https://github.com/shivamkumard107/AtmanirbharCompiler/issues">Report Bug</a>
    ·
    <a href="https://github.com/shivamkumard107/AtmanirbharCompiler/issues">Request Improvements</a>
  </p>
</p>



<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
  * [EBNF Code](#ebnf-code)
  * [Built With](#built-with)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Running and Compiling](#running-and-compiling)
* [Example Testcases](#example-testcases)
* [Contributing](#contributing)
* [License](#license)
* [Contributors](#contributors)
* [Contact](#contact)
* [Acknowledgements](#acknowledgements)



<!-- ABOUT THE PROJECT -->
## About The Project
A compiler is a program that converts instructions into a machine-code or lower-level form so that they can be read and executed by a computer.

This(actually every) Compiler is build into two sections known as frontend and backend. 

Frontend contains:
* Lexical analyzer
* Syntax analyzer
* Semantic analysis 
* Intermediate code generator 

Backend contains:
* Code optimizer 
* Code generator

The Atmanirbhar compiler is divided into three parts. In the frontend we build lexical and syntax analyzer giving output an intermediate form and backend contains code generator which outputs assembly code finally. :smile:
#### EBNF Code
Extended Backus–Naur form (EBNF) is a family of metasyntax notations, any of which can be used to express a context-free grammar. EBNF is used to make a formal description of a formal language such as a computer programming language. They are extensions of the basic Backus–Naur form (BNF) metasyntax notation. 
[![EBNFcode][TextFile]](https://github.com/shivamkumard107/AtmanirbharCompiler/EBNFcode)

### Built With
* [C language](https://en.cppreference.com/w/c/language)
* [EBNF](https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_form)


<!-- GETTING STARTED -->
## Getting Started

We will first compile the Lexial analyzer, Syntax analyzer and Code generator using gcc

### Prerequisites

* gcc
```sh
sudo apt update

sudo apt install build-essential

sudo apt-get install manpages-dev

gcc --version
```

### Running and Compiling

1. Compile the Lexical, Syntax analyzer and Code generator
```sh
gcc Code/Lexical.c -o lex
gcc Code/Syntax.c -o syntax
gcc Code/CodeGen.c -o codegen
```

2. Use the given testcases(testcase1.txt, testcase2.txt) as an input in lex analyzer and redirect output to lexOutput.txt
```sh
./lex testcase1.txt > lexOut.txt
```

3. Lexial analyzer outputs stream of tokens which consists of identifier, keywords,separator,operator, and literals which goes as an input in Syntax analyzer 
```sh
./syntax lexOut.txt > synOut.txt
```

4. Syntax analyzer outputs a parse tree build from the grammar defined by EBNF(disscussed above) which goes as an input in Code generator
```sh
./codegen synOut.txt > codeGen.txt
```

<!-- USAGE EXAMPLES -->
## Example Testcases

* Sample Testcase 1
```C
count = 5;
c = 'a';
 while (count < 100) {
     print("count is: ", count, "\n");
     count = count + 5;
 }
```
Output for above testcase:
* [Lexical Output(token stream)](https://github.com/shivamkumard107/AtmanirbharCompiler/testcases/testcase1/lexout1.txt)
* [Syntax Output(parse tree)](https://github.com/shivamkumard107/AtmanirbharCompiler/testcases/testcase1/synout1.txt)
* [Code generator(Machine Code)](https://github.com/shivamkumard107/AtmanirbharCompiler/testcases/testcase1/codegenout1.txt)

* Sample Testcase 2
```C
/*
  Comments supported
 */
number = 5;

if(number<10){

	print(number, "\n");
}
```
Output for above testcase:
* [Lexical Output(token stream)](https://github.com/shivamkumard107/AtmanirbharCompiler/testcases/testcase2/lexout2.txt)
* [Syntax Output(parse tree)](https://github.com/shivamkumard107/AtmanirbharCompiler/testcases/testcase2/synout2.txt)
* [Code generator(Machine Code)](https://github.com/shivamkumard107/AtmanirbharCompiler/testcases/testcase2/codegenout2.txt)



<!-- CONTRIBUTING -->
## Contributing

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.

## Contributors

* **Shivam Kumar** - *Initial work* - [@shivamkumard107](https://github.com/shivamkumard107)
* **Shashank Kumar** - *Initial work* - [@shashank](https://github.com/sneakatyou)
* **Shikhar Malik** - *Initial work* - [@shikharmaxx](https://github.com/shikharmaxx) 

<!-- CONTACT -->
## Contact

Shivam Kumar - [@shivamkumard107](https://www.linkedin.com/in/shivam-kumar-a9aa96131/) - shivam.kumard107@gmail.com

<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements
* [Rosettacode](https://rosettacode.org/wiki/Compiler)
* [Shashank Kumar](https://daneden.github.io/animate.css)
* [Shikhar Malik](https://connoratherton.com/loaders)
* [Sumesh Pandit](https://pages.github.com)
* [othneildrew](https://github.com/othneildrew/Best-README-Template)
* [Jonathan Engelsma](https://youtu.be/54bo1qaHAfk)
* [Wikipedia](https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_form)