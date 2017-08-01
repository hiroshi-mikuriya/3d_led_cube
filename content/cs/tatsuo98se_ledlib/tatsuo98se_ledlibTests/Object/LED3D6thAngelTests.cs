using Microsoft.VisualStudio.TestTools.UnitTesting;
using LEDLIB;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LEDLIB.Tests
{
    [TestClass()]
    public class LED3D6thAngelTests
    {
        [TestMethod()]
        public void NormlizeTest_MinusRadian()
        {
            var target = new LED3D6thAngel();
            Assert.AreEqual(1.5 * Math.PI, target.Normlize(-Math.PI / 2));
        }

        [TestMethod()]
        public void NormlizeTest_MultRadian()
        {
            var target = new LED3D6thAngel();
            Assert.AreEqual(Math.PI, target.Normlize(3 * Math.PI));
        }

        [TestMethod()]
        public void GetTriangleColorTest()
        {
            var target = new LED3D6thAngel();
            var c1 = target.GetTriangleColor(0, Math.PI * 0);
            var c2 = target.GetTriangleColor(0, Math.PI * 0.5);
            var c3 = target.GetTriangleColor(0, Math.PI * 1);
            var c4 = target.GetTriangleColor(0, Math.PI * 1.5);
            var c5 = target.GetTriangleColor(0, Math.PI * 2);


        }
    }
}