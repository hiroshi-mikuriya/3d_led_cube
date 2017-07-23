using Microsoft.VisualStudio.TestTools.UnitTesting;
using LEDLIB;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LEDLIB.Tests
{
    [TestClass()]
    public class RGBTests
    {
        [TestMethod()]
        public void fromColorTest()
        {
        }

        [TestMethod()]
        public void RGBTest()
        {
            var target = new RGB(-1, -1, -1);
            Assert.AreEqual(0, target.R);
            Assert.AreEqual(0, target.G);
            Assert.AreEqual(0, target.B);
        }

        [TestMethod()]
        public void ToIntTest()
        {
        }

        [TestMethod()]
        public void ToColorTest()
        {
        }
    }
}