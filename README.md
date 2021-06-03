# ❄️ frost
frost programming language


## About

forst is a programming language with a focus on low-friction systems programming.

## Example
```

import("io");

@debuggable
@serialisable
Person : type {
    name : String;
    age  : U8;
}

main : pub fn (args : []String){

    Person^ person = new Person{
        args[0],
        args[1].to_str()
    };

    io.println(person);

    delete person;

}

```
# 🔨 building
frost uses CMake as the build system, currently ninja is used as the target generator

##  windows
```
cd toolchain
build_and_run.bat
```
## linux
```
cd toolchain
build_and_run.sh
```