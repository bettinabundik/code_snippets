using System;

namespace KnightsTour
{
    class Program
    {
        static void Main(string[] args)
        {
            KnightsModel model = new KnightsModel(4);

            //model.GenerateDIMACS();
            model.DecodeSATResults("asd.txt");
        }
    }
}
