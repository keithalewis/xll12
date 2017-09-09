struct C {
    int operator()(int i)
    {
        return 2*i;
    }
    int twice(int i)
    {
        return 2*i;
    }
};
...

C c;
c(2); // -> 4
c.operator()(2); // -> 4
c.twice(2); // -> 4