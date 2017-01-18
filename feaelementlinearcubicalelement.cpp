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
    baseMass_ = new double *[Nunknown];
    baseStiff_ = new double *[Nunknown];
    for (int i = 0; i < Nunknown; i++) {
        baseMass_[i] = new double[Nunknown];
        baseStiff_[i] = new double[Nunknown];
    }

    // not general (XX, YY, ZZ for local coordinate only)
    double a = geometry()->e(GeometryEle::X);
    double b = geometry()->e(GeometryEle::Y);
    double c = geometry()->e(GeometryEle::Z);

    double v = 0; // from material type
    double E = material()->E();
    double rho = material()->rho();

    double v22 = (1.0 - 2.0*v)/2.;
    double abc8 = a*b*c/8.0;
    double N[8];
    double Nx[8];
    double Ny[8];
    double Nz[8];
    mathExtension::vectorZero<double,8>(N);
    mathExtension::vectorZero<double,8>(Nx);
    mathExtension::vectorZero<double,8>(Ny);
    mathExtension::vectorZero<double,8>(Nz);
    double N1[6][24]; // 6 nDOF, 24 known // not zerized
    double N2[3][24];
    mathExtension::matrixZero<double,6,24>(N1);
    mathExtension::matrixZero<double,3,24>(N2);
    double Q[6][6] =\
    {
        {1-v,  v,   v,   0, 0, 0},\
        {v,  1-v,  v,   0, 0, 0},\
        {v,   v,  1-v,  0, 0, 0},\
        {0,  0,  0, v22,  0,   0},\
        {0,  0,  0,  0,  v22,  0},\
        {0,  0,  0,  0,   0,  v22}\
    };

    for(int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            Q[i][j] *= E/(1.0+v)/(1.0 - 2*v);
        }
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

                for (int l = 0; l < 6; l++) {
                    // l==0
                    if(l==0){
                        int c = 0;
                        for (int m = 0; m < 24; m=m+3) {
                            N1[l][m] = Nx[c];
                            c++;
                        }
                    }

                    // l==1
                    if(l==1){
                        int c = 0;
                        for (int m = 1; m<24; m=m+3) {
                            N1[l][m] = Ny[c];
                            c++;
                        }
                    }

                    // l==2
                    if(l==2){
                        int c = 0;
                        for (int m = 2; m<24; m=m+3) {
                            N1[l][m] = Nz[c];
                            c++;
                        }
                    }

                    // l==3
                    if(l==3){
                        int c = 0;
                        for (int m = 1; m<24; m=m+3) {
                            N1[l][m] = Nz[c];
                            c++;
                        }
                        c = 0;
                        for (int m = 2; m<24; m=m+3) {
                            N1[l][m] = Ny[c];
                            c++;
                        }
                    }

                    // l==4
                    if(l==4) {
                        int c = 0;
                        for (int m = 0; m<24; m=m+3) {
                            N1[l][m] = Nz[c];
                            c++;
                        }
                        c = 0;
                        for (int m = 2; m<24; m=m+3) {
                            N1[l][m] = Nx[c];
                            c++;
                        }
                    }

                    // l==5
                    if(l==5){
                        int c = 0;
                        for (int m = 0; m<24; m=m+3) {
                            N1[l][m] = Ny[c];
                            c++;
                        }
                        c = 0;
                        for (int m = 1; m<24; m=m+3) {
                            N1[l][m] = Nx[c];
                            c++;
                        }
                    }
                }


                // find stiff matrix
                for (int i = 0; i < 24; i++) {
                    for (int j = 0; j < 24; j++) {
                        baseStiff_[i][j] += N1[j][i]*Q[i][j]*N1[i][j]*weight;// double check!!!
                    }
                }

                // find mass matrix
                for (int i = 0; i < 3; i++) {

                    // i==0
                    if(i==0) {
                        int c = 0;
                        for (int j = 0; j < 24; j=j+3){
                            N2[i][j] = N[c];
                            c++;
                        }
                    }

                    // i==1
                    if(i==1) {
                        int c = 0;
                        for (int j = 1; j < 24; j=j+3){
                            N2[i][j] = N[c];
                            c++;
                        }
                    }

                    // i==2
                    if(i==2) {
                        int c = 0;
                        for (int j = 2; j < 24; j=j+3){
                            N2[i][j] = N[c];
                            c++;
                        }
                    }
                }

                // find mass matrix
                for (int i = 0; i < 24; i++) {
                    for (int j = 0; j < 24; j++) {
                        baseMass_[i][j] += N2[j][i]*(N2[i][j]*weight*rho);// double check!!!
                    }
                }

            }
        }
    }

    // coordinate system transformation
    // from local to global

    const coordSystem * cs = geometry()->localCoordinateSystem();
    const QVector3D & e0 = cs->e0();
    const QVector3D & e1 = cs->e1();
    const QVector3D & e2 = cs->e2();

}

FEAElementLinearCubicalElement::~FEAElementLinearCubicalElement()
{
    int N = nNode*nDOF;
    for (int i = 0; i < N; i ++){
        delete [] baseMass_[i];
        delete [] baseStiff_[i];
    }
    delete [] baseMass_;
    delete [] baseStiff_;
}

makeElement(ElementName, \
            FEAElementLinearCubicalElement, \
            FEAElementThreeD, \
            LinearCubicalElementBarThreeD)
