# PLT-Project-2025

Fichier de rapport : `report.pdf`

Pour exécuter ce programme :

```sh
# Assure that you're currently in the root directory of this folder
make
```

Vous observerez une séquence de sorties:

```sh 
mkdir -p build
gcc -g -Wall -Wextra -c src/lex/analex.c -o build/analex.o
gcc -g -Wall -Wextra -c src/lex/test_analex.c -o build/test_analex.o
gcc -g -Wall -Wextra -o build/test_analex build/analex.o build/test_analex.o
gcc -g -Wall -Wextra -c src/syn/anasynt.c -o build/anasynt.o
gcc -g -Wall -Wextra -c src/syn/test_anasynt.c -o build/test_anasynt.o
gcc -g -Wall -Wextra -o build/test_anasynt build/anasynt.o build/test_anasynt.o build/analex.o
gcc -g -Wall -Wextra -c src/sem/anasem.c -o build/anasem.o
gcc -g -Wall -Wextra -c src/sem/test_anasem.c -o build/test_anasem.o
gcc -g -Wall -Wextra -o build/test_anasem build/anasem.o build/test_anasem.o build/anasynt.o build/analex.o
gcc -g -Wall -Wextra -c src/compil/compilateur.c -o build/compilateur.o
gcc -g -Wall -Wextra -c src/compil/test_compilateur.c -o build/test_compilateur.o
gcc -g -Wall -Wextra -c src/runtime/runtime.c -o build/runtime.o
gcc -g -Wall -Wextra -o build/test_compilateur build/compilateur.o build/test_compilateur.o build/analex.o build/anasynt.o build/anasem.o build/runtime.o
gcc -g -Wall -Wextra -c src/runtime/test_runtime.c -o build/test_runtime.o
gcc -g -Wall -Wextra -o build/test_runtime build/runtime.o build/test_runtime.o build/compilateur.o build/analex.o build/anasynt.o build/anasem.o
```

Aucun avertissement n'existe dans le code ! Nous sommes très fiers de nous ;)

Après avoir construit les exécutables, pour testez les différentes parties :

```sh
# L'analyseur lexical
build/test_analex testfiles/Regles.txt

# L'analyseur syntaxique
build/test_anasynt testfiles/Regles.txt

# L'analyseur sémantique
build/test_anasem testfiles/Regles.txt

# La compilateur
build/test_compilateur testfiles/Regles.txt

# La VM
build/test_runtime testfiles/Init.txt
```

Arborescence après la construction des fichiers d'exécution:
.
├── Makefile
├── README.md
├── report.pdf
├── src
│   ├── compil
│   │   ├── compilateur.c
│   │   ├── compilateur.h
│   │   └── test_compilateur.c
│   ├── global.h
│   ├── lex
│   │   ├── analex.c
│   │   ├── analex.h
│   │   └── test_analex.c
│   ├── runtime
│   │   ├── runtime.c
│   │   ├── runtime.h
│   │   └── test_runtime.c
│   ├── sem
│   │   ├── anasem.c
│   │   ├── anasem.h
│   │   └── test_anasem.c
│   └── syn
│       ├── anasynt.c
│       ├── anasynt.h
│       └── test_anasynt.c
└── testfiles
    ├── Init.txt
    ├── Regles.txt
    └── Unicodes.txt

8 directories, 23 files
