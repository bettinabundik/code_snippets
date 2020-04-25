using System;
using System.Collections.Generic;

namespace KnightsTour
{
    public class Position
    {
        public Int32 x;
        public Int32 y;

        public Position()
        {
            x = 0;
            y = 0;
        }

        public Position(Int32 _x, Int32 _y)
        {
            x = _x;
            y = _y;
        }
    }

    public class Variable
    {
        public Int32 i;
        public Int32 j;
        public Int32 k;
        public Boolean value;

        public Variable(Int32 _i, Int32 _j, Int32 _k, Boolean _value)
        {
            i = _i;
            j = _j;
            k = _k;
            value = _value;
        }
    }
    
    public class KnightsModel
    {
        private Int32 n;
        private List<List<Variable>> cnf1;
        private List<List<Variable>> cnf2;
        private List<List<Variable>> cnf3;
        private List<List<Variable>> cnf4;
        private List<Int32> move_x;
        private List<Int32> move_y;

        public KnightsModel (Int32 _n)
        {
            n = _n;
            
            cnf1 = new List<List<Variable>>();
            cnf2 = new List<List<Variable>>();
            cnf3 = new List<List<Variable>>();
            cnf4 = new List<List<Variable>>();

            move_x = new List<Int32>();
            move_y = new List<Int32>();

            move_x.Add(2);
            move_x.Add(1);
            move_x.Add(-1);
            move_x.Add(-2);
            move_x.Add(-2);
            move_x.Add(-1);
            move_x.Add(1);
            move_x.Add(2);

            move_y.Add(1);
            move_y.Add(2);
            move_y.Add(2);
            move_y.Add(1);
            move_y.Add(-1);
            move_y.Add(-2);
            move_y.Add(-2);
            move_y.Add(-1);
        }

        public void GenerateDIMACS()
        {
            CNF_Constraint1();
            CNF_Constraint2();
            CNF_Constraint3();
            CNF_Constraint4();
            WriteAll();
        }

        private Boolean ValidCell(Position p)
        {
            return p.x >= 1 && p.y >= 1 && p.x <= n && p.y <= n;
        }

        private Int32 ConvertToNumber(Int32 i, Int32 j, Int32 k)
        {
            return 100 * i + 10 * j + k; //n * n * i + n * j + k;
        }

        private void ConvertTo_3CNF(ref List<List<Variable>> cnf)
        {
            List<List<Variable>> cnf_new = new List<List<Variable>>();
            Int32 count_i = n * n + 1;
            Int32 count_j = n + 1;
            Int32 count_k = n + 1;

            for (int i = 0; i < cnf.Count; i++)
            {
                if (cnf[i].Count <= 3)
                    cnf_new.Add(cnf[i]);
                else
                {
                    List<Variable> tmp = new List<Variable>();
                    tmp.Add(cnf[i][0]);
                    tmp.Add(cnf[i][1]);
                    tmp.Add(new Variable(count_i, count_j, count_k, true));
                    cnf_new.Add(tmp);

                    for (int j = 2; j < cnf[i].Count; j++)
                    {
                        List<Variable> tmp2 = new List<Variable>();

                        count_i++;
                        count_j++;
                        count_k++;

                        tmp2.Add(new Variable(count_i, count_j, count_k, false));
                        tmp2.Add(cnf[i][j]);
                        tmp2.Add(new Variable(count_i + 1, count_j + 1, count_k + 1, true));

                        cnf_new.Add(tmp2);
                    }
                }
            }

            cnf = new List<List<Variable>>(cnf_new);
        }
        
        public void CNF_Constraint1()
        {
            // at time i, one element of the list is true, others are false
            // list of 2-CNF variables
            // for every i
            // not x (i,j,k) or not x (i, j', k') = true
            // for every (j,k) and (j',k') pairs where j != j' and k != k'

            for (int i = 1; i <= n * n; i++)
            {
                Variable knightpos = new Variable(i, (int)Math.Ceiling((double)i / n), i % n, false);

                for (int j = 1; j <= n; j++)
                {
                    for (int k = 1; k <= n; k++)
                    {
                        if (j != knightpos.j && k != knightpos.k)
                        {
                            List<Variable> tmp = new List<Variable>();
                            tmp.Add(knightpos);
                            tmp.Add(new Variable(i, j, k, false));
                            cnf1.Add(tmp);
                        }
                    }
                }
            }
        }
        
        public void CNF_Constraint2()
        {
            // knight's valid movements
            // knight at time i, at position (j,k)
            // at time i+1, at least 1 out of 8 cells is true
            // from 3-CNF to 9-CNF variables
            // not x (i,j,k) or   OR [ x (i+1,j',k') ]
            //                    for every (j',k') != (j,k)

            for (int i = 1; i <= n * n - 1; i++)
            {
                Variable knightpos = new Variable(i, (int)Math.Ceiling((double)i / n), i % n, false);

                List<Variable> tmp = new List<Variable>();
                tmp.Add(knightpos);

                for (int m = 0; m < move_x.Count; m++)
                {
                    Position next = new Position(knightpos.j + move_x[m], knightpos.k + move_y[m]);
                    if (ValidCell(next))
                        tmp.Add(new Variable(i + 1, next.x, next.y, true));
                }

                cnf2.Add(tmp);
            }

            ConvertTo_3CNF(ref cnf2);
        }

        public void CNF_Constraint3()
        {
            // every (j,k) position visited only once
            // list of 2-CNF variables
            // for every (j,k)
            // not x (i,j,k) or not x (i', j, k) = true
            // for every i and i' time where i != i'

            for (int j = 1; j <= n; j++)
            {
                for (int k = 1; k <= n; k++)
                {
                    for (int i1 = 1; i1 <= n * n; i1++)
                    {
                        Variable x = new Variable(i1, j, k, false);
                        for (int i2 = i1 + 1; i2 <= n * n; i2++)
                        {
                            List<Variable> tmp = new List<Variable>();
                            tmp.Add(x);
                            tmp.Add(new Variable(i2, j, k, false));
                            cnf3.Add(tmp);
                        }
                    }
                }
            }
        }

        public void CNF_Constraint4()
        {
            // no cell is left unvisited
            // N^2-CNF variables
            // OR [ x (i,j,k) ] for every i,j,k

            List<Variable> tmp = new List<Variable>();
            for (int i = 1; i <= n * n; i++)
            {
                for (int j = 1; j <= n; j++)
                {
                    for (int k = 1; k <= n; k++)
                    {
                        tmp.Add(new Variable(i, j, k, true));
                    }
                }
            }

            cnf4.Add(tmp);

            ConvertTo_3CNF(ref cnf4);
        }

        private void ConvertToDIMACS(List<List<Variable>> cnf, ref List<String> dimacs)
        {
            for (int i = 0; i < cnf.Count; i++)
            {
                String row = "";
                for (int v = 0; v < cnf[i].Count; v++)
                {
                    String variable;
                    if (cnf[i][v].value)
                        variable = ConvertToNumber(cnf[i][v].i, cnf[i][v].j, cnf[i][v].k).ToString();
                    else
                        variable = "-" + ConvertToNumber(cnf[i][v].i, cnf[i][v].j, cnf[i][v].k).ToString();

                    row += variable + " ";
                }

                row += "0";
                dimacs.Add(row);
            }
        }

        public void WriteAll()
        {
            List<String> dimacs = new List<String>();

            ConvertToDIMACS(cnf1, ref dimacs);
            ConvertToDIMACS(cnf2, ref dimacs);
            ConvertToDIMACS(cnf3, ref dimacs);
            ConvertToDIMACS(cnf4, ref dimacs);

            using (System.IO.StreamWriter file =
                new System.IO.StreamWriter("knightstour.dimacs"))
            {
                file.WriteLine("p cnf 1 2");
                for (int i = 0; i < dimacs.Count; i++)
                {
                    file.WriteLine(dimacs[i]);
                }
            }
        }

        public void DecodeSATResults(String result_filename)
        {
            String[] variables;

            using (System.IO.StreamReader file =
                new System.IO.StreamReader(result_filename))
            {
                file.ReadLine(); // "SAT"

                String text = file.ReadLine();
                Char[] whitespace = new Char[] { ' ', '\t' };
                variables = text.Split(whitespace);
                for (int i = 0; i < variables.Length; i++)
                    if (variables[i][0] != '-')
                        Console.WriteLine(variables[i]);
                Console.ReadKey();
            }

            //List<Variable> result = new List<Variable>();

            //Console.WriteLine("Result variables of SAT solver\n");
            //for (int i = 0; i < variables.Length; i++)
            //{
            //    if (variables[i][0] != '-')
            //    {
            //        Console.WriteLine(variables[i]);

            //        Int32 _i = Int32.Parse(variables[i]) / (n * n);
            //        Int32 _j = (Int32.Parse(variables[i]) - _i) / n;
            //        Int32 _k = (Int32.Parse(variables[i]) - (_i * n * n) - (_j * n));
            //        result.Add(new Variable(_i, _j, _k, true));
            //    }
            //}
            //Console.ReadKey();

            //for (int i = 0; i < variables.Length; i++)
            //{

            //}
        }
    }
}
