using System;
using System.Collections.Generic;

namespace CSharpCoroutine
{
    class Program
    {
        public static void Main(string[] args)
        {
            IEnumerable<int> Source(int max)
            {
                int first = 1, second = 1;
                Console.WriteLine($"Source Show:{first} ");
                yield return first;
                Console.WriteLine($"Source Show:{second} ");
                yield return second;
                for (int i = 0; i < max; ++i)
                {
                    int third = first + second;
                    first = second;
                    second = third;
                    Console.WriteLine($"Source Show:{third} ");
                    yield return third;
                }
            }

            int iTimes = 0;
            foreach (int i in Source(8))
            {
                Console.WriteLine($"Main Show:{i},Time:{++iTimes}");
            }
            Console.WriteLine();
        }
    }
}
