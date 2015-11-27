# xml2json

xml2json is a tool to transform XML files under an specific format to C++ json parser.

With the new Boom C++ is having at the moment thanks to C++11/C++14 and the upcoming C++17 I felt there was something
missing in this language. Working on cloud applications is not trivial, and usually involves several parts and components.
REST APIs are the common mechanism to communicate against the servers, and the underlying content is JSON, so after creating
your application model, data structures, etc... comes one big and annoying part, to parse the JSON message.

JSON messages should not be difficult to read by humans, and pretty lightweight for computers, that makes them the perfect
choice to transmit data, and thanks to its "tree" structure, allows developers express quite complex models in an easy way.
That friendly and human like behavior of the JSON has a non-desired effect, we include a lot of fields.

So this library is as NHibernate is for SQL, but for JSON. We will create an xml model of our data, that will create
and parse automatically the data we want to read.

### Version

1.0.0

### Requirements

To be able to compile the project you need a version of [boost][1] that supports program options, I'm using 1.58 at the moment.

### Installation

To install the tool you just need clone the repository on a local folder

```sh
$ git clone [git-repo-url] [folder-you-want]
```

Create a bin folder and call cmake

```sh
$ mkdir bin
$ cd bin
$ cmake ../
```

### Usage

In order to make use of this program I recommend first take a look at the help output

```sh
$ ./umison -h
umison allowed options:
  -h [ --help ]               produce this help message
  -e [ --write-file-h ] arg   The .h output will be written to this file
  -p [ --write-file-cpp ] arg The .cpp output will be written to this file
  -i [ --read-file ] arg      Use this file as input xml template
  -a [ --append-string ] arg   String to be appended in the internal namespace
                              to avoid collisions with existing code
```

By default the program uses the standard input so you can redirect it like in the following case

```sh
$ cat test.xml | ./umison
```

To produce an output.

By default the program will use the standard output but you can specify different files for the generated code

```sh
$ cat test.xml | ./umison
```

```cpp
/**
 * This file has been created automatically. Any change will be lost
 * Please modify your input template to create a different output
 * instead of modifying this file.
 *
 * Author: José Gerardo Palma Durán
 *
 * Disclaimer: The author of this software is not responsible of any
 * possible damage/problem caused by the usage of this software. This
 * code is provided as is, without any warranty.
 *
 * If you want to report any bug, please contact me at jpalma at barracuda dot com
 *
 * If you didn't get a copy of the code used to create these templates, you can always
 * download it for free from https://github.com/raistmaj/xml2json
 * */

#include <string>
#include <vector>

// Internal namespace declaration
namespace __internal__umison {

    // Forward declaration
```

You can specify a file as input template and files to print the output, remember if you only specify one of the files
the other will use the standard output.

```sh
$ ./umison -i test.xml -e test.h -p test.cpp
XML parsing complete.
XML checking minimum requirements.
Done.
Checking references on Classes and Jsons.
Parsing complete.
Starting to build output.
$ ls
umison  test.cpp  test.h
```

### Template format

As mentioned before we use XML. XML was used as the structure is pretty close to json and additionally we can include
attributes allowing us to specify names, conditions, etc...

The document must start with the root tag *umison*

```xml
<umison>
</umison>
```

At this level you can specify two possible entities, *class* and *json*, the first one is used to reference data structures
by the rest, the second one is the parsers we want to generate and will be exposed on the header file.

```xml
<umison>
    <class>
        ...
    </class>
    ...
    <json>
        ...
    </json>
</umison>
```

One important note about classes and references is that the code will be created in the same order you specify on your
template, so as in C/C++ you can't use an undefined data structure and the objects are designed to use RAII(no pointers
are involved so automatic memory management and possible compile time optimizations).

#### Data types and attributes

We must identify basic type(the ones that are supported by the parser automatically), and composed types(the ones built
with multiple types).

Basic types are:

* integer: 64 bits integer
* int32: 32 bits integer, the reason of this names is because I wanted the default be 64 bits
* float: a floating point number.
* string: a string value
* list: a Json array
* refclass: a named object which references a complex type.
* map: an anonymous object where its name is not given in the json. One important note about maps is, they are the only
allowed type on one level, we expect that level to be only a map. Maps are distinguish from the rest as the first
element is a string and is not known at compile time, as consequence will be filled with the retrieved result,
this is important so the solution to avoid collisions is to forbid different elementson that level. At the moment is
user responsibility to avoid maps with other elements on one level within the xml.

```json
{
    "22039182" : {
        ...
    }
}
```

In that fragment 22039182 is the user ID but we don't have something like this

```json
{
    "user_data" : {
        "user_id" : 22039182,
        "internal_user_data" : {
            ...
        }
    }
}
```

Inside *class* and *json* we can use the following table to guide us on wich attributes are supported by each tag

| Data type / Attribute | optional | condition | name | refclass | value |
|-----------------------|----------|-----------|------|----------|-------|
| integer               | X        | X         | X    |          |       |
| int32                 | X        | X         | X    |          |       |
| float                 | X        | X         | X    |          |       |
| string                | X        | X         | X    |          |       |
| list                  | X        | X         | X    | X        |       |
| refclass              | X        | X         | X    | X        |       |
| map                   | X        | X         | X    |          | X     |

Each attribute will have a different default value, and in the case of name, if we leave it empty, the behavior is undefined.
On list and refclass if we left empty refclass attribute the behavior is undefined too.

* optional: False by default, will mark if the node can be skipped in the parsing. An error will be reported if the node
exists and the type is wrong.
* condition: Empty by default, C/C++ code to be evaluate within an if condition to consider if we must read that node
* name: Name we want to use to identify the element in our data structure and in the json, is 1-1
* refclass: Class to be used in the referenced elements.
* value: Type we want to use on the map

The next types are supported by the refclass attribute

* integer: The array/class will be an integer of 64 bits
* int32: The array/class will be an integer of 32 bits
* float: The array/class will be a float
* string: The array/class wll be a string
* class name: The array/class will be a class already defined in your template

For a complete example please see the file test.xml or the test folder.

The final representation depends on the output engine.

### Output engines

The system requires what is known as output engines to produce our .h and .cpp, depending on the one you select the results
may differ(the .cpp).

The .h is always the same, will not be different no matter what, as this is a C++ creator we use STL for the data structures
if necessary.

The next list shows the conversion from xml type to cpp type

* integer: `long long int`
* int32: `int`
* float: `double`
* string: `std::string`
* list: `std::list<refType>`
* map: `std::multimap<std::string,refType>`

### Tested

The software and resulting files have been tested on Linux but should work on Windows.

The main binary is kinda fragile and will abort under some scenarios.

The resulting json parser MUST be pretty solid and avoid

LICENSE
----

See LICENSE.txt for a copy of the license



[1]:http://www.boost.org/