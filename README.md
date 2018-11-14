# dot
## Dynamic Object Typification

There is a lot of cases when we have no idea what type of variables comes to us. Every time you use SQL-query or call RPC-method via REST API you may know what data type come back on your query or may not. Even order of fields in SQL-query results or minor type changes on server-side able to corrupt your application logic written on C++ with usage of data like optional, tuple, variant, any and so on.

DOT is the new way of solutions for such cases. All you need is object with runtime determined data instance which is derived from object::data base class. You may work with suitable classes derived from object base class that provide methods to handle such data. For example object may contain data with text and you always be able to handle such data with text class or handle any scalar data with scalar class.

### Hierarchy of classes

- object
	- scalar<type\>
	- copyable<type\>

### Hierarchy of data classes are same

- object::data
	- scalar<type\>::data
	- copyable<type\>::data

### Description
Any class hierarchy provide the way to check is class derived from another and check is instance implements any class form this hierarchy. It works as well even with RTTI switched off. Any hierarhy provide id of any class inside that hierarchy and any instance of such class may tell what class it implements without dynamic_cast or typeid usage with simplest additional object per class.

Data is strictly limited by size to place into internal buffer inside object class instance. If you need any data bigger than pair of 64-bit integers please use copyable template which is derived from object and use only one pointer as data stored in buffer inside object. Base object class is optimized for scalar operations and pass by value if it is required, so any decimal data or big integer data will use no more than pair of 64-bit integers as well. Any scalar data is OK to be stored inside internal object buffer. One additional place in buffer is reserved for virtual function table pointer which is in any data class derived from object::data and obviously use virtual methods.

This library will grow up step by step and goes richer by classes, documentation and tests.
Feel free for any requests, usage or forks and good luck to us.

Each file which contains source code also contains the ending line with Unicode signature to ensure that your code editor supports correct Unicode representation and all you see is correct. The ending line is the same for each source code file and contains author name of source code in native language using Cyrillic and always looks like the ending line in this README.md. You may check how it's shown in your browser for example and compare it with your source code editor representation.

// Unicode signature: Владимир Керимов
