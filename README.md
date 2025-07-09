<div align="center">
<table>
    <thead>
        <tr>
            <td style="width:25%; text-align:center;"><img src="/img/epis.png" alt="EPIS" style="width:80%; height:auto"/></td>
            <td style="text-align:center;">
                <span><b>UNIVERSIDAD NACIONAL DE SAN AGUSTIN</b></span><br />
                <span><b>FACULTAD DE INGENIERÍA DE PRODUCCIÓN Y SERVICIOS</b></span><br />
                <span><b>DEPARTAMENTO ACADÉMICO DE INGENIERÍA DE SISTEMAS E INFORMÁTICA</b></span><br />
                <span><b>ESCUELA PROFESIONAL DE INGENIERÍA DE SISTEMAS</b></span>
            </td>
            <td style="width:25%; text-align:center;"><img src="/img/abet.png" alt="ABET" style="width:80%; height:auto"/></td>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td colspan="3"><span><b>Curso</b></span>: Programación de Sistemas</td>
        </tr>
        <tr>
            <td colspan="3"><span><b>Fecha</b></span>: 30/07/2025</td>
        </tr>
    </tbody>
</table>
</div>
<div align="center" style="margin-top: 10px;">
    <img src="/img/unsa.png" alt="UNSA" width="450px" height="150px">
    <h1 style="font-weight:bold; font-size: 2em;">DBGS (Datos Base Gestor Sistema)</h1>
</div>

DBGS es un sistema gestor de bases de datos extremadamente simple, limitado en funcionalidad y un prototipo experimental. Funciona completamente en memoria y ofrece una interfaz de consola interactiva con sintaxis similar a SQL separada por pipas.

## Características

- **Gestión de Tablas**: Crear, insertar, seleccionar y eliminar registros
- **Estructuras de Índices**: B+ Trees, Hash Maps e Índices Invertidos
- **Consola Interactiva**: Interfaz de línea de comandos similar a SQL
- **Arquitectura Modular**: Sistema bien estructurado con componentes separados
- **Documentación**: Documentación generada con Doxygen

## Estado del Proyecto

✅ **Completo (?)**

El proyecto cuenta con:
- Sistema de construcción con CMake
- Estructura de datos implementada (B+ Trees, Hash Maps)
- Parser SQL básico con sintaxis de pipas
- Tests unitarios (`TableTest`, `ParsingTest`)
- Documentación generada automáticamente
- Diagramas UML de la arquitectura

## Instalación y Configuración

### Prerrequisitos

- GCC/Clang (C++17 o superior)
- CMake (versión 3.10+)
- Opcional: devenv (para entorno de desarrollo reproducible)

### Instalación Rápida

```bash
# Clonar el repositorio
git clone <url-del-repositorio>
cd dbgs-psis

# Construir el proyecto
./build.sh

# Ejecutar la consola interactiva
./build/Main
```

### Instalación con devenv (Recomendado para desarrollo)

```bash
# Devenv
curl -L https://devenv.sh/install.sh | sh

# Entrar al entorno de desarrollo
devenv shell

# Construir el proyecto
./build.sh
```

## Uso

### Consola Interactiva

Ejecuta `./build/Main` para iniciar la consola interactiva:

```
Welcome to the DBGS-PSIS console. Type 'exit' or 'quit' to leave.
DBGS> CREATE TABLE users (id INT, name STRING, age INT)
DBGS> INSERT INTO users VALUES (1, "Juan", 25)
DBGS> SELECT * FROM users WHERE age > 20
DBGS> exit
```

### Tests

```bash
# Ejecutar tests de tabla
./build/TableTest

# Ejecutar tests de parsing
./build/ParsingTest
```

### Documentación

La documentación se genera automáticamente con Doxygen:

```bash
doxygen Doxyfile
```

## Arquitectura

### Componentes Principales

- **Parser**: Analizador SQL con sintaxis de pipas (`src/system/Parser.hpp`)
- **Table**: Gestión de tablas en memoria (`src/system/Table.hpp`)
- **Estructuras de Datos**:
  - B+ Tree (`src/structures/BPlusTree.hpp`)
  - Hash Map (`src/structures/HashMap.hpp`)
  - Índice Invertido (`src/structures/InvertedIndex.hpp`)

### Estructura del Proyecto

```
src/
├── Main.cpp              # Punto de entrada de la consola
├── system/               # Componentes del sistema
│   ├── Parser.*         # Analizador SQL
│   ├── Table.*          # Gestión de tablas
│   └── IndexEntry.*     # Entradas de índice
└── structures/          # Estructuras de datos
    ├── BPlusTree.hpp    # Árbol B+
    ├── HashMap.hpp      # Mapa hash
    └── InvertedIndex.*  # Índice invertido
```

## Desarrollo

### Herramientas de Desarrollo

El proyecto utiliza las siguientes herramientas:
- **CMake**: Sistema de construcción
- **devenv**: Entorno de desarrollo reproducible con Nix
- **Doxygen**: Generación de documentación
- **PlantUML**: Diagramas UML
- **GCC/Clang**: Compiladores con C++17

## Autores

- CACERES RUIZ, JOHANN ANDRE
- JARA MAMANI, MARIEL ALISSON
- NOA CAMINO, YENARO JOEL
- MESTAS ZEGARRA, CHRISTIAN RAUL
- VALDIVIA SEGOVIA, RYAN FABIAN

## Licencia

El proyecto se encuentra bajo los términos de la [Licencia MIT](/LICENSE)
