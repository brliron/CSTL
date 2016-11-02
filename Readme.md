I've seen some people saying C++ is better than C because you don't have to handle memory allocation, things like std::vector does it for you. But the difference between C and C++ isn't in something like a std::vector or a std::list. They aren't part of the language syntax, they belongs to a mere library (yeah, that's the standard library, but that's still a library). From my point of view, C VS C++ is only about coding style: imperative or object programming? (plus a few fancy things like operator overloading or templates)
And that's how I came to this project. C++ is nice because you have the STL containers? Here is the C STL, a C implementation of the STL.

This project is not a full implementation of the STL, it's rather a proof of concept: my goal isn't to provide a STL port in C, it's to prove that the STL containers aren't what makes C++ great, that they are totally doable in C.
In that goal, the syntax have to be light, even if we need an heavy implementation behind to support this syntax.


### Build
I may include some build scripts someday, but for now, `gcc your_source_files.c cstl/*.c` is the way to go.

### Usage

Lots of use cases are shown in example.c.
2 containers are implemeted: vector and list.
C doesn't have templates, so I dealt with macros instead. To use a container for a given type, define `CSTL_NAME` as the suffix of the container's name and `CSTL_TYPE` as the type inside the container, then include the corresponding header file. For example, if you want a vector of ints called vectornumber, define `CSTL_TYPE` to int and `CSTL_NAME` to number, then include vector.h. `CSTL_TYPE` and `CSTL_NAME` are automatically undefined, you can give them another value then re-include vector.h to define another vector.

To create a new container, declare a pointer to the container and call `container_name_new()`. For example:
```c
vectornumber* v = vectornumber_new();
```
All containers have a delete member function:
```c
v->delete();
```
Vector have a push_back member function:
```c
v->push_back(3);
```
List have 2 member functions. If l is a list of ints:
```c
l->push_front(3);
l->push_back(3);
```
All containers support for_each:
```c
for_each(int, n, v, printf("%d\n", n));
```
The 4th parameter of for_each can either be an instruction or a block:
```c
for_each(int, it, v,
{
    int var;
    var = it + 1;
    printf("%d + 1 = %d\n", it, var);
});
```


### Compatibility notes
I tested this project under the following environments:
- MinGW (gcc 4.9.3) on a Windows 7 64 bits
- MinGW-w64 (gcc 5.3.0) on a Windows 7 64 bits
- Visual Studio 2015 (x86 and x64 builds) on a Windows 7 64 bits
- gcc 6.2.1 on a Linux 4.8.4 64 bits
It may work on different OSes (I think it should work at least on Windows 10 and Linux 32 bits).
It may or may not work on different compilers (for example, it didn't work on Visual Studio at first, see commit 82f8b29589dd96c51a07bcf93674eca985b3bde2).

### TODO
- Procect against double inclusion with the same name.
- Add std::string and std::map.
- Allow the user to allocate the container on the stack instead of using a pointer, and provide automatic cleanup when the object is removed from the stack.
