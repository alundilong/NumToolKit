#ifndef MATHEXTENSIONIO_H
#define MATHEXTENSIONIO_H

std::ostream &operator<<(std::ostream& os, const mathExtension::Matrix &m)
{
    int nr = m.nrow();
    int nc = m.ncol();
    for(int i = 0; i < nr; i++ ) {
        for(int j = 0; j < nc; j++) {
            os << m[i][j] << ' ';
        }
        os << '\n';
    }
    return os;
}


#endif // MATHEXTENSIONIO_H
