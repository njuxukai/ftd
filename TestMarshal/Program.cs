using System;
using System.Runtime.InteropServices;

using Xcp;

namespace TestMarshal
{
    class Program
    {
        static void Main(string[] args)
        {
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
