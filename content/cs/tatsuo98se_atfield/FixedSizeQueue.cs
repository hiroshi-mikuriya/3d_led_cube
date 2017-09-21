using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HandsViewer.cs
{
    public class FixedSizedQueue<T>
    {
        List<T> q = new List<T>();
        private object lockObject = new object();

        public FixedSizedQueue()
        {
            this.Limit = 5;
        }

        public FixedSizedQueue(int limit)
        {
            this.Limit = limit;
        }

        public T this[int index]
        {
            get
            {
                return q[index];
            }

        }

        public T lastObject()
        {
            if(q.Count == 0)
            {
                return default(T);
            }
            return q[q.Count - 1];
        }
        public T lastlastObject()
        {
            if (q.Count <= 2)
            {
                return default(T);
            }
            return q[q.Count - 2];
        }
        public int Limit { get; set; }
        public void Enqueue(T obj)
        {
            q.Add(obj);
            lock (lockObject)
            {
                if(q.Count > Limit)
                {
                    q.RemoveAt(0);
                }
            }
        }
    }
}
