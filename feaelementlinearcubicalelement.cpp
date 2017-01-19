#include "feaelementlinearcubicalelement.h"


const int FEAElementLinearCubicalElement::nNode = 8;
const int FEAElementLinearCubicalElement::nDOF = 3;

FEAElementLinearCubicalElement::FEAElementLinearCubicalElement()
{
}

FEAElementLinearCubicalElement::FEAElementLinearCubicalElement\
(\
        const std::string &dimension, \
        const std::string &name, \
        const MaterialEle &m, \
        const GeometryEle &g
)
    : FEAElementThreeD(dimension, name, m, g)
{
    log_ = "3D LinearCubicalElement is selected \n";
    dim_ = 3;
    nDOFEle_ = 3;
    nNodeEle_ = 8;
    name_ = "LinearCubicalElement3D";
    log_ += QString("%3D LinearCubicalElement : nNode = %2 : DOF = %3 \n").arg(dim()).arg(nNodeEle()).arg(nDOFEle());

    const int Nunknown = nNode*nDOF;
//    baseMass_ = new double *[Nunknown];
//    baseStiff_ = new double *[Nunknown];
//    for (int i = 0; i < Nunknown; i++) {
//        baseMass_[i] = new double[Nunknown];
//        baseStiff_[i] = new double[Nunknown];
//    }

    baseMass_ = mathExtension::Matrix(Nunknown,Nunknown);
    baseStiff_ = mathExtension::Matrix(Nunknown,Nunknown);

    // not general (XX, YY, ZZ for local coordinate only)
    double a = geometry()->e(GeometryEle::X);
    double b = geometry()->e(GeometryEle::Y);
    double c = geometry()->e(GeometryEle::Z);

    double v = 0; // from material type
    double E = material()->E();
    double rho = material()->rho();

    double v22 = (1.0 - 2.0*v)/2.;
    double abc8 = a*b*c/8.0;
    mathExtension::Vector N(8);
    mathExtension::Vector Nx(8);
    mathExtension::Vector Ny(8);
    mathExtension::Vector Nz(8);

    mathExtension::Matrix N1(6,24);
    mathExtension::Matrix N2(3,24);

    double QQ[6][6] = \
    {
        {1-v,  v,    v,   0,   0,     0},\
        {v,    1-v,  v,   0,   0,     0},\
        {v,    v,    1-v, 0,   0,     0},\
        {0,    0,    0,   v22, 0,     0},\
        {0,    0,    0,   0,   v22,   0},\
        {0,    0,    0,   0,   0,   v22}\
    };

    for(int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            QQ[i][j] *= E/(1.0+v)/(1.0 - 2*v);
        }
    }

    mathExtension::Matrix Q(6,6);
    mathExtension::Vector V(6);
    for(int i = 0; i < Q.nrow(); i++) {
        for (int j = 0; j < Q.ncol(); j++) {
            V.set(i,QQ[i][j]);
        }
        Q.set(i,V);
    }

    // define stiff/mass matrix
    for (int i = 0; i < 2; i++) {
        long double x = mathExtension::gaussP(2, i);
        for (int j = 0; j < 2; j++) { //
            long double y = mathExtension::gaussP(2, j);
            for (int k = 0; k < 2; k++) {
                long double z = mathExtension::gaussP(2, j);

                long double wx = mathExtension::gaussW(2, i);
                long double wy = mathExtension::gaussW(2, j);
                long double wz = mathExtension::gaussW(2, k);
                long double weight = wx*wy*wz*abc8;
                long double xy = x*y; long double yz = y*z; \
                long double zx = z*x; long double xyz = x*y*z;

                N[0] = (1-x-y-z+xy+yz+zx-xyz)/8.0;
                N[1] = (1+x-y-z-xy+yz-zx+xyz)/8.0;
                N[2] = (1+x+y-z+xy-yz-zx-xyz)/8.0;
                N[3] = (1-x+y-z-xy-yz+zx+xyz)/8.0;
                N[4] = (1-x-y+z+xy-yz-zx+xyz)/8.0;
                N[5] = (1+x-y+z-xy-yz+zx-xyz)/8.0;
                N[6] = (1+x+y+z+xy+yz+zx+xyz)/8.0;
                N[7] = (1-x+y+z-xy+yz-zx-xyz)/8.0;

                Nx[0] = (-1+y+z-yz)/4.0/a;
                Nx[1] = (1-y-z+yz)/4.0/a;
                Nx[2] = (1+y-z-yz)/4.0/a;
                Nx[3] = (-1-y+z+yz)/4.0/a;
                Nx[4] = (-1+y-z+yz)/4.0/a;
                Nx[5] = (1-y+z-yz)/4.0/a;
                Nx[6] = (1+y+z+yz)/4.0/a;
                Nx[7] = (-1-y-z-yz)/4.0/a;

                Ny[0] = (-1+x+z-zx)/4.0/b;
                Ny[1] = (-1-x+z+zx)/4.0/b;
                Ny[2] = (1+x-z-zx)/4.0/b;
                Ny[3] = (1-x-z+zx)/4.0/b;
                Ny[4] = (-1+x-z+zx)/4.0/b;
                Ny[5] = (-1-x-z-zx)/4.0/b;
                Ny[6] = (1+x+z+zx)/4.0/b;
                Ny[7] = (1-x+z-zx)/4.0/b;

                Nz[0] = (-1+y+x-xy)/4.0/c;
                Nz[1] = (-1+y-x+xy)/4.0/c;
                Nz[2] = (-1-y-x-xy)/4.0/c;
                Nz[3] = (-1-y+x+xy)/4.0/c;
                Nz[4] = (1-y-x+xy)/4.0/c;
                Nz[5] = (1-y+x-xy)/4.0/c;
                Nz[6] = (1+y+x+xy)/4.0/c;
                Nz[7] = (1+y-x-xy)/4.0/c;

                mathExtension::pos colPos;
                colPos = {1, 3, 24};
                N1.setColValues(0, colPos, Nx);
                colPos = {2, 3, 24};
                N1.setColValues(1, colPos, Ny);
                colPos = {3, 3, 24};
                N1.setColValues(1, colPos, Nz);

                colPos = {2, 3, 24};
                N1.setColValues(3, colPos, Nz);
                colPos = {3, 3, 24};
                N1.setColValues(3, colPos, Ny);

                colPos = {1, 3, 24};
                N1.setColValues(4, colPos, Nz);
                colPos = {3, 3, 24};
                N1.setColValues(4, colPos, Nx);

                colPos = {1, 3, 24};
                N1.setColValues(5, colPos, Ny);
                colPos = {2, 3, 24};
                N1.setColValues(5, colPos, Nx);

                baseStiff_ = baseStiff_ + N1.transpose()*Q*N1*weight;

                // prepare for mass matrix
                colPos = {1, 3, 24};
                N2.setColValues(0, colPos, N);
                colPos = {2, 3, 24};
                N2.setColValues(1, colPos, N);
                colPos = {3, 3, 24};
                N2.setColValues(2, colPos, N);

                // find mass matrix
                baseMass_ = baseMass_ + N2.transpose()*(N2*weight*rho);

            }
        }
    }

    // coordinate system transformation
    // from local to global

    const coordSystem * cs = geometry()->localCoordinateSystem();
    const QVector3D & e0 = cs->e0();
    const QVector3D & e1 = cs->e1();
    const QVector3D & e2 = cs->e2();
    const QVector3D eg0 = QVector3D(1,0,0);
    const QVector3D eg1 = QVector3D(0,1,0);
    const QVector3D eg2 = QVector3D(0,0,1);

    mathExtension::Matrix G(24,24);
    int step = 3;
    for (int i = 0; i < 24; i = i+step) {
        mathExtension::pos rowRange = {1+step,1,3+step};
        mathExtension::pos colRange = {1+step,1,3+step};
        mathExtension::Matrix T(3,3);
        //T = ;
        T[0][0] = QVector3D::dotProduct(eg0, e0); T[0][1] = QVector3D::dotProduct(eg0, e1); T[0][2] = QVector3D::dotProduct(eg0, e2);
        T[1][0] = QVector3D::dotProduct(eg1, e0); T[1][1] = QVector3D::dotProduct(eg1, e1); T[1][2] = QVector3D::dotProduct(eg1, e2);
        T[2][0] = QVector3D::dotProduct(eg2, e0); T[2][1] = QVector3D::dotProduct(eg2, e1); T[2][2] = QVector3D::dotProduct(eg2, e2);
        G.setSubMatrix(rowRange, colRange, T);
    }

    baseStiff_ = G.transpose()*baseStiff_*G;
    baseMass_ = G.transpose()*baseMass_*G;


}

FEAElementLinearCubicalElement::~FEAElementLinearCubicalElement()
{
//    int N = nNode*nDOF;
//    for (int i = 0; i < N; i ++){
//        delete [] baseMass_[i];
//        delete [] baseStiff_[i];
//    }
//    delete [] baseMass_;
//    delete [] baseStiff_;
}

makeElement(ElementName, \
            FEAElementLinearCubicalElement, \
            FEAElementThreeD, \
            LinearCubicalElementBarThreeD)
