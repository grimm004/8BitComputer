using System;
using System.Collections.Generic;
using System.IO;

namespace ConsoleApplication24
{
    class Program
    {
        static void Main(string[] args)
        {
            string[] readFile = File.ReadAllLines(@"\Documents\Control word text.txt");
            List<string> output = new List<string>();
            foreach (string line in readFile)
            {
                string[] addressWord = line.Split(',');
                int count = 0;
                foreach (char c in addressWord[0])
                {
                    if (c == 'x') count++;
                }
                for (int i = 0; i < Math.Pow(2, count); i++)
                {
                    string result = Convert.ToString(i, 2);
                    int lenSave = result.Length;
                    for (int j = 0; j < count - lenSave; j++)
                    {
                        result = "0" + result;
                    }
                    int charIndex = 0;
                    string address = "";
                    for (int j = 0; j < addressWord[0].Length; j++)
                    {
                        if (addressWord[0][j] == 'x')
                        {
                            address += result[charIndex];
                            charIndex++;
                        }
                        else address += addressWord[0][j];
                    }
                    output.Add(address + "," + addressWord[1]);
                }
            }
            File.WriteAllLines(@"Documents\Control word text II.txt", output.ToArray());
        }
    }
}
