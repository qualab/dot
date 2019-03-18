# dot
## Dynamic Object Typification

There is a lot of cases when we have no idea what type of variables comes to us. Every time you use SQL-query or call RPC-method via REST API you may know what data type is going to come back on your query or may not. Even order of fields in SQL-query results or minor type changes on server-side able to corrupt your application logic written on C++ with usage of data like optional, tuple, variant, any and so on.

Using dot:: types is the new way of solutions for such cases. All you need is object with runtime determined data instance which is derived from object::data base class. You may work with suitable classes derived from object base class that provide methods to handle such data. For example object may contain data with text and you always be able to handle such data with text class or handle any scalar data with scalar class.

### The hierarchy of classes

- object
	- boxes
		- box<slim\>
	- ropes
		- rope<fat\>

### The hierarchy of data classes is the same

- object::data
	- boxes::cats
		- box<slim\>::cat
	- ropes::cows
		- rope<fat\>::cow

## dot::object
This is not usual C++ class, instance of object may contain data of any derived class. Another words you may freely write something like:

`object rose = flower(ROSE); object Alfred = dog("Alfred");`

This class also is suitable for abstraction from type, so any native C++ type may freely placed into any instance of `dot::object`:

`object count = 12; object truth = true; object weight = 76.5;`

Such common types are very comfortable for usage in collections where instances and types are determined at runtime by incoming data sets. For example data records returned from SQL-query or result of remote procedure call (RPC). Any abstraction from compile time type are no more requires pointers or endless type casts.

Any instance of dot::object may extract value of any type if such convertation is exist or you may require to instantiate template method object::get_as<type>(). This method is already exist for native C++ types and for derived classes like `dot::box` or `dot::rope`. Use this method directly or use any way of C++ type cast:

`int number = static_cast<int>(count); double value = weight.get_as<double>();`

Default constructor of `dot::object` creates object with no value also known as `null`. It is valid case to check is the object null or not:

`object nothing; if (nothing.is_null()) { ... } if (truth.is_not_null()) { ... }`

## dot::box
Any object has data incapsulated and stored in the internal buffer which size is limited by the pair of 64-bit values. In case when the data of derived is fit to be placed into this buffer that is boxing and that is `dot::box` class template. Data class of `dot::box` is called `cat` that is liquid enough to be placed into the `box`. Instance of `cat` contains value as-is and it is good way to place any native scalar into such `box`:

`box<long long> num = 100500LL; box<float> coef = 2.71828f;`

Use of `box` means that you know the type of data stored, so you can freely call look() or touch() to retrieve const or non-const reference to the value stored in the `box`.

`coef.touch() *= 1.1f; return num.look();`

Any `box<type>` is derived from intermediate class `dot::boxes` as well as any `box<type>::cat` is derived from class `dot::boxes::cats`. That is helpful node of hierarchy to determine is the box class o instance derived from object in case when you received an object but do not know the type. Simply check is as follows:

`object result = remote::call(argument); if (result.is<boxes>()) { ... }`

## dot::rope
Instead of `box` class `rope` refers the huge data that cannot be in-placed into the internal buffer of `object`. Class called `rope` because of data of this class implements Copy-on-Write pattern or C-o-W. To this is reference to `cow` called `rope`. Copy-on-Write pattern means many reference of `rope` may be bound to one `cow` and call any const method but first call of non-const method from any `rope` will follow the clone creation to change it own copy without corruption of shared `cow`. All methods of stored value type are calling via `operator ->` which determine const and non-const method calling: 

`rope<matrix3x3> matrix = rotation(angles); return matrix->det();`

Copy-on-Write pattern implemented to all C++ lexical string types. So any object instantiated from `std::string` or `const wchar_t*` contains the `rope::cow` of proper type. 

`rope<string> name = u8"Маруся"; rope<u32string> address = U"деревня Гадюкино";`

## Description
Any class hierarchy provide the way to check is class derived from another and check is instance implements any class form this hierarchy. It works as well even when RTTI is switched off. Any hierarchy tree provides the identification for any class inside that hierarchy and any instance of such class may tell what class it implements without dynamic_cast or typeid usage with simplest additional object per class.

The object's data is strictly limited by size to place that into the internal buffer inside the object class instance. If you need any data bigger than pair of 64-bit integers please use dot::rope template which is derived from object and uses only one pointer as data stored in buffer inside object. Base object class is optimized for scalar operations and for being passed by value if it is required, so any decimal data or big integer data will use no more than pair of 64-bit integers as well. Any scalar data is OK to be stored inside internal object buffer use dot::box derived class then. One additional place in buffer is reserved for virtual function table pointer which is in any data class derived from object::data and obviously use virtual methods.

This library will grow up step by step and goes richer by classes, documentation and tests.
Feel free for any requests, usage or forks and good luck to us.

### By the way
Each file which contains source code also contains the ending line with Unicode signature to ensure that your code editor supports correct Unicode representation and all you see is correct. The ending line is the same for each source code file and contains author name of source code in native language using Cyrillic and always looks like the ending line in this README.md. You may check how it's shown in your browser for example and compare it with your source code editor representation.

// Здесь должен быть Unicode
