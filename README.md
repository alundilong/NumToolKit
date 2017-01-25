NumToolKit - Numerical Simulation ToolKit

NumTooKit is an undergoing numerical simulation toolbox covering FEA/SPH/CFD analysis.

*FEA package targets at performing nonlinear structure analysis of higly flexible material.
High Performance Parallel Computing tech with timer-triggered domain decompostion will be
implemented to conduct challenging simulation that involves mass destruction in the system,
and others such as additive manufacturing simulaiton.

*SPH package simulates Fluid, Solid and Fluid-Structural Interation problems within the
Smoothed-Particel-Hydrodynamics technique, this package is developed based on LAMMPS SPH package.
Our package functions in solid/fluid/FSI simulation, which LAMMPS's SPH does not well support.

*CFD package will be coming ... (the package partially share with FEA)

Contributor : YJ Mao, ymao.mu@gmail.com


======================== example code =============================

<!--construct 3D element from FEAElementThreeD-->

    std::unique_ptr<FEAElementThreeD> parentEle = \
    FEAElementThreeD::New(\
    "ThreeD",\
    "LinearCubicalElementBarThreeD",\
    *m,\
    *g);

<!--construct 3D element from FEAElementBase-->
    std::unique_ptr<FEAElementBase> parentEle = \
    FEAElementBase::New(\
    "ThreeD",\
    "LinearCubicalElementBarThreeD",\
    *m,\
    *g);

====================================================================
//  construct mesh with polyMesh

    const Mesh & polyMesh = (*mesh());
    const QList<QList<int> > &cellNode = polyMesh.cellNode();

    const int nElement = polyMesh.nCells();
    const int nUnknown = polyMesh.nNodes()*3;

    std::string nameMat = "Aluminum-2014T";
    QList<FEAElementLinearCubicalElement*> elements;
    for (int i = 0; i < nElement; i++) {
        MaterialEle *m = new MaterialEle(nameMat);
        const QList<int> & vertex = cellNode[i];
        GeometryEle *g = new GeometryEle(polyMesh, vertex);
        std::unique_ptr<FEAElementBase> parentEle = \
                FEAElementBase::New(\
                    "ThreeD",\
                    "LinearCubicalElementBarThreeD",\
                    *m,\
                    *g);
        FEAElementLinearCubicalElement *lce =\
                static_cast<FEAElementLinearCubicalElement*>(parentEle.get());
        parentEle.release();
        elements.push_back(lce);
    }

    mathExtension::Matrix A(nUnknown , nUnknown);
    mathExtension::Vector b(nUnknown);
    mathExtension::Vector x(nUnknown);

    QList<FEAElementLinearCubicalElement*>::const_iterator it;
    qDebug() << "====== Form Linear Algebra Equations =====";
    for(it = elements.begin(); it != elements.end(); ++it) {
        const FEAElementLinearCubicalElement &ele = **it;
        const QList<int> &Rows= ele.nodeIds();
        // be aware of vertex id (our id starts from 0)
        A.assemblyMatrix(Rows, Rows, ele.baseStiff(),false, ele.nDOF); // index with no moveby
    }

    setBoundaryConditions(polyMesh,A,b);
    linearAlgebraSolver las(A, b, x);
    las.LUSolve_GSL();
    // post-processing
    // output new positions
    // output ux, uy, uz
    writeData(polyMesh, x);
