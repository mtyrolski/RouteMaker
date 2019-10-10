## RouteMaker
The project below presents a complex tool for handling national road maps. The code contains 3 modules.

## Modules
Below are listed main modules of project and their brief description. Details are in sections corresponding to modules' names<br>
**1. Interface map.h and additional files** - provides A-Z tools which allows fully control the road map<br>
**2. Query parser map_main.c** - parses for executing commands in form of `command;param_1;param_2;...;param_n`<br>
**3. Script map.sh** - calculates total lenghts of routes <br>
and additionally:<br>
**4.** *Testing scripts, valgrind, dataset for testing*

## Definitions
Before describing specific modules there should be assumed following definitions:
- Route map is set of cities which are connected by roads.
- City is represented by name which is non-empty `cstring` without chars with ASCII coded from 0 to 31, without `;` and finished with zero.
- Each segment of road connects 2 different cities. Between 2 cities there can be at most 1 road segment. 
- Road segment contains attributes: length <a href="https://www.codecogs.com/eqnedit.php?latex=$\in$&space;\mathbb&space;Z_&plus;" target="_blank"><img src="https://latex.codecogs.com/gif.latex?$\in$&space;\mathbb&space;Z_&plus;" title="$\in$ \mathbb Z_+" /></a> and year <a href="https://www.codecogs.com/eqnedit.php?latex=\in&space;\mathbb&space;Z&space;\setminus&space;\left\{&space;0&space;\right\}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\in&space;\mathbb&space;Z&space;\setminus&space;\left\{&space;0&space;\right\}" title="\in \mathbb Z \setminus \left\{ 0 \right\}" /></a> which is the year of construction or the year of the last renovation (assumption thatthere  was not `0` year)
- Route is an sequence of roads which connects different cities without gaps, self-cuts and loops.
- National route is a route. One road section may belong to many national routes. A national route is identified by its number, which is an integer between 1 and 999. The length of the national road is the sum of the lengths of its sections.

## Interface
Interface map.h provedies functions described below:<br>

| Function | T(n) | M(n) |
|     :---:      |     :---:      |     :---:      |
| Map* newMap(...)   |  O(1)  |  O(1)   |
| bool addRoad(...)   |  O(log(c))   |  O(1)   |
| bool repairRoad(...)   |  O(log(c))   |  O(1)   |
| bool newRoute(...)  |  O(r * log(c) + R)   |  O(c)   |
| bool extendRoute(...)  |  O(R + r * log(c))   |  O(c)   |
| bool removeRoad(...)   |  O(c + R + r * log(c))   |  O(c)   |
| char const* getRouteDescription(...)   |  O(R+c)   |  O(c)   |
| bool constructNewRoute(...) |  O(n)   |  O(n)  |
| bool removeRoute(...)   |  O(R)   |  O(1)   |

<br>

Each of them is **in great detail** descibed in `map.h`.

```
c := cities number in current map
n := cities number in upcoming request
r := roads number
R := routes number
```

## Query parser
Allows to calls program for concrete functions and also implements additional functionality.

---

`id of route;city name;road length;road year;city name;road length;road year;city name;…;city name` <br> <br>
The command format is the same as the result of the getRouteDescription function. This command creates a national road with the given number and route. If a city or road section does not exist, it creates it. If a road section already exists, but has an earlier year of construction or last renovation, it modifies this attribute of the road section. We consider it a mistake if the road section already exists but has a different length or later year of construction or last renovation. This command prints nothing to standard output.

---

```c
addRoad;city1;city2;length;builtYear
repairRoad;city1;city2;repairYear
getRouteDescription;routeId
newRoute;routeId;city1;city2
extendRoute;routeId;city
removeRoad;city1;city2
removeRoute;routeId
```
These commands calls corresponding functions from `map.h` with given parameters.

---

## Script map.sh
Script which first argument is a path to file with results of `getRouteDesciption`, each result in separate lines. Next (optional) arguments are indexes of national routes. For each question calculates total lengths of routes and prints to output message in format:
`route idx;length`. <br> <br>
If there is no information about route, script will print nothing. Assumption that for each route there is at most 1 information.

## How to use
```bash
git clone https://github.com/mvxxx/RouteMaker.git
cd RouteMaker
mkdir release
cd release
cmake ..
make
```
After that each module can be used as described in that readme.
## Tests
There are 2 additional scripts for testing project. It uses mainly query parser but since parser uses `map.h` it generally tests each functionality of project. <br>

`test_output.sh` - takes each `.in` file from given path and compare output with `.out` and `.err` file. <br>
`test_valgrind.sh` - takes each `.in` file from given path, passes program via `valgrind` and checks if there is no memory problems. <br><br>

Dataset for testing can be found in link: https://www.mediafire.com/file/7lvfkj91r29coo2/tesciory-3.zip/file. 
There are 960 files with 4.652.570 lines of reqests in total.
<br><br>
Average time for over **1 million** requests is about 20 seconds (= 50.000/s) on `AMD RYZEN 5 2600`.
## Docs
Whole project is documented using `Doxygen`. To generate documentation type `make doc`. <br><br>You should have installed `Doxygen` and its dependencies. 

