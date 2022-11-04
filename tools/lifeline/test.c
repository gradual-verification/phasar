void test (int ** p, int * q){
    *p = q;
}

int main() {
    int x = 1;
    int y = 1;
    int * a = &x;
    int * b = &y;
    int ** c = &a;
    test(c, b);
    return 0;
}

