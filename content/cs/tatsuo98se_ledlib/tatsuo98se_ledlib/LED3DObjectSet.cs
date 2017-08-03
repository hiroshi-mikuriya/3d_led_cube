namespace LEDLIB
{
    public class LED3DObjectSet
    {
        public LED3DObject obj;
        public ILED3DCanvas filter;

        public LED3DObjectSet(LED3DObject obj, ILED3DCanvas filter)
        {
            this.obj = obj;
            this.filter = filter;
        }

        public LED3DObjectSet(LED3DObject obj)
            : this(obj, null)
        {

        }
    }

}
