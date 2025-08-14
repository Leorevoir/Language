//INFO: not working yet
/**
* a `struct` is a collection of data
*
* `struct` can have methods<
*
* at construction, the `init` method is called
* at destruction, the `destroy` method is called
*
* you can use '@' keyword to pass parameters by reference and access them
*/

struct Person {

    name: string;
    age: int;

    /**
    * @brief constructor for the Person struct
    *
    * theses parameters doesn't need to be typed, because they are assigned at construction
    * to their eponymous fields
    */
    def init(@name, @age) -> null
    {
        print("created person: " + @name + ", age: " + @age);
    }

    def greet() -> null
    {
        print("hello " + name + "!");
    }

    /**
    * @brief destructor for the Person struct
    */
    def destroy() -> null
    {
        print("destroying person: " + name);
    }

}

def main() -> int
{
    p = Person("Alice", 30);

    p.greet();
}
