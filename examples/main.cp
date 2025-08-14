 // encapsuler des modules comme étant des méta classes statiques non allouées, comme en Ruby
module maths {

    // structures semi OOP
    struct Vec2
    {
        // jaimais bien le x: float;
        x: float;
        y: float;
    
        // comme en Crystal, initialisation directe: out_x va se map à in_x à l'aide du keyword @
// le type de @x @y est donc sécurisé par la déclaration privée de la structure comme étant float, pas besoin de le réécrire en gros
        def init(@x, @y) -> null
        {
        }

        def length() -> float
        {
            return sqrt(x*x + y*y);
        }
    }

}

// soit on fait main() en entry-point, soit un fait un langage scriptable avec any en entry point
def main() -> int
{
    for i in 10 { // prc ça me soule d'écrire des for in range
      print(i); // print ou "show" ?
    }
    counter = 0; // auto typed to "int"
    forever { // while true
        ++counter; // je veux mon ++i
        break if counter == 4;  // prc le ruby c génial
    }
    return 0;
}
