using System;
using System.Drawing;

namespace LEDLIB
{
    abstract class LED3DObject
    {
        private DateTime bornAt;
        private TimeSpan lifeTime;
        private RGB color;

        public LED3DObject(RGB color) : this(color, TimeSpan.Zero)
        {
        }

        public LED3DObject(RGB color, TimeSpan lifeTime)
        {
            this.color = color;
            this.bornAt = DateTime.Now;
            this.lifeTime = lifeTime;
        }

        public TimeSpan GetElapsedAt()
        {
            return DateTime.Now.Subtract(this.bornAt);
        }

        public RGB Color
        {
            get
            {
                return this.color;
            }
        }

        virtual public bool IsExpired()
        {
            if(lifeTime == TimeSpan.Zero)
            {
                return false;
            }
            var now = DateTime.Now;

            return (now - this.bornAt > this.lifeTime);
        }

        abstract public void Draw(LED3DCanvas canvas);
    }
}
