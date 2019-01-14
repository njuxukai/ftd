using System;
using System.Runtime.InteropServices;

using Xcp;

namespace TestMarshal
{
    struct A
    {
        public int x;
    };
    class Program
    {
        static void Main(string[] args)
        {
             
            A v = new A();
            v.x = 20;
            A? a = v;

            A? b = a;
            v.x = 10;
            a = v;
            Console.WriteLine("{0},{1}", a.Value.x, b.Value.x);
            ReqUserLoginField field = new ReqUserLoginField();
            Console.WriteLine(Marshal.SizeOf(field));
            Trader trader = new Trader();
            while (1 == 1)
            {
                Console.ReadLine();
            }
        }
    }
}
