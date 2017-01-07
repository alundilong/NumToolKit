#ifndef FEAELEMENTBASE_H
#define FEAELEMENTBASE_H

#include <QString>

/*
 * abstract class for finite element
 */

class FEAElementBase
{
public:
    FEAElementBase();
    ~FEAElementBase();
    QString getLog() {return log_;}

    // public access
    const int & dim() const { return dim_; }
    const int & nDOF() const { return nDOF_;}
    const int & nSize() const { return nSize_;}
    const QString & name() const { return name_;}
    double** baseMass() const { return baseMass_;}
    double** baseStiff() const { return baseStiff_;}

private:

protected:
    int dim_;  // dimension of element
    int nDOF_; // Degree of Freedom
    int nSize_; // Size of characteristic Matrix
    QString name_; // name of element
    QString log_; // report log

    double **baseMass_; // mass matrix
    double **baseStiff_; // stiffiness matrix
};

#endif // FEAELEMENTBASE_H
