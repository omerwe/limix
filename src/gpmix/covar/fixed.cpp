/*
 * fixed.cpp
 *
 *  Created on: Nov 11, 2011
 *      Author: stegle
 */
#include "fixed.h"
#include "gpmix/types.h"
#include "assert.h"

namespace gpmix {



gpmix::CFixedCF::CFixedCF(const MatrixXd & K0) : ACovarianceFunction(1)
{
	this->K0 = K0;
}


gpmix::CFixedCF::~CFixedCF()
{
}


void gpmix::CFixedCF::Kcross(MatrixXd *out, const CovarInput & Xstar) const
{
	mfloat_t A = exp((mfloat_t)((2.0 * params(0))));
	(*out) = A * this->K0cross;
}

void CFixedCF::K(MatrixXd *out) const
{
	mfloat_t A = exp((mfloat_t)((2.0 * params(0))));
	(*out) = A * this->K0;
}


void gpmix::CFixedCF::Kgrad_param(MatrixXd *out, const muint_t i) const
{
	mfloat_t A = exp((mfloat_t)((2.0 * params(0))));

	if (i==0)
	{
		(*out) = 2.0 * A * this->K0;
	}
}

void gpmix::CFixedCF::Kcross_grad_X(MatrixXd *out, const CovarInput & Xstar, const muint_t d) const
{
	(*out) = MatrixXd::Zero(X.rows(),Xstar.rows());
}

void gpmix::CFixedCF::Kdiag_grad_X(VectorXd *out, const muint_t d) const
{
	(*out) = VectorXd::Zero(X.rows());
}

MatrixXd CFixedCF::getK0() const
{
	return K0;
}


MatrixXd CFixedCF::getK0cross() const
{
	return K0cross;
}

void CFixedCF::setK0(const MatrixXd& K0)
{
	this->K0 = K0;
}

void CFixedCF::setK0cross(const MatrixXd& Kcross)
{
	this->K0cross = Kcross;
}



void gpmix::CFixedCF::getK0(MatrixXd *out) const
{
	(*out) = K0;
}



void gpmix::CFixedCF::getK0cross(MatrixXd *out) const
{
	(*out) = K0cross;
}



/* namespace gpmix */
}
