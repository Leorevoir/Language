//INFO: not working yet
// /**
// * example of a recursive function to calculate factorial
// * you don't need to specify the type of the parameters if and only if you set a default value
// *
// *  C+ supports ternary operator and if | else syntax
// */
// TODO:
// def factorial(n = 5) -> int
// {
//     return 1 ? n <= 1 : n * factorial(n - 1);
// }
//INFO: working syntax
def factorial(n: int) -> int
{
    if n <= 1 {
        return 1;
    }
    return n * factorial(n - 1);
}

/**
* main function to call the factorial function
*
* `result` doesn't need to be typed, because it is assigned at construction
*/
def main() -> int
{
    result = factorial();
    print("Factorial of " + n + " is: " + result);
}
