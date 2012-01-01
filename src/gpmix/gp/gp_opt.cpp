/*
 * CGPopt.cpp
 *
 *  Created on: Jan 1, 2012
 *      Author: stegle
 */

#include "gp_opt.h"

namespace gpmix {

CGPopt::CGPopt(CGPbase& gp) : gp (gp)
{
	tolerance = 1E-4;
	// TODO Auto-generated constructor stub

}

CGPHyperParams CGPopt::getParams() const
{
	return params;
}

CGPHyperParams CGPopt::getFilter() const
{
	return filter;
}

void CGPopt::opt()
{
	//1. get starging point
	VectorXd x = gp.getParamArray();
	muint_t numParams = x.rows();
	//2. create optimization instance:
	nlopt_opt opt = nlopt_create(NLOPT_LD_LBFGS, numParams);
	nlopt_set_min_objective(opt, CGPopt::gptop_nlopt_objective, this);
	//3. set tolerance
	nlopt_set_xtol_rel(opt, tolerance);
	//4. set constraints
	//TODO
	//nlopt_add_inequality_constraint(opt, myconstraint, &data[0], 1e-8);

	double* x0d = x.data();
	double minf; /* the minimum objective value, upon return */
	if (nlopt_optimize(opt, x0d, &minf) < 0) {
	    printf("nlopt failed!\n");
	}
	else {
		//1. reevaluate at optimum
		VectorXd df;
		double lml_opt = objective(x,&df);
		//2. diagonoses:
		std::cout << "optimum found for: f(x=["<<x<<"]) = "<< lml_opt << "\n";
	    std::cout << "df(x)=[" << df << "]\n";
	    std::cout << "----------" << "\n";
	}
}

void CGPopt::setFilter(CGPHyperParams filter)
{
	this->filter = filter;}

void CGPopt::setParams(CGPHyperParams params)
{
	this->params = params;
}

CGPopt::~CGPopt() {
	// TODO Auto-generated destructor stub
}

double CGPopt::objective(const VectorXd& paramArray)
{
	double lml;
	//set Params
	gp.setParamArray(paramArray);
	lml = gp.LML();
	return lml;
}


double CGPopt::objective(const VectorXd& paramArray,VectorXd* gradParamArray)
{
	double lml;
	//set Params
	gp.setParamArray(paramArray);
	lml = gp.LML();
	gp.aLMLgrad(gradParamArray);
	return lml;
}

double CGPopt::getTolerance() const
    {
        return tolerance;
    }

    void CGPopt::getTolerance(double tol)
    {
        this->tolerance = tol;
    }

    double CGPopt::gptop_nlopt_objective(unsigned  n, const double *x, double *grad, void *my_func_data)
    {
    	double lml;
        //1. cast additional data as Gptop
        CGPopt *gpopt = (CGPopt*)(my_func_data);
        //2. map parameters and gradients
        Eigen::Map<const VectorXd > vX(x,n);

        //need gradient?
        if (grad)
        {
        	VectorXd vgrad;
        	lml = gpopt->objective(vX,&vgrad);
        	//copy:
        	Eigen::Map<VectorXd > vG(grad,n);
        	vG = vgrad;
        	/*
        	std::cout << "f("<< vX << ")="<< lml << "\n";
        	std::cout << "df = "<<vgrad << "\n\n";
        	*/
        }
        else
        	lml = gpopt->objective(vX);
        //std::cout << "f("<< vX << ")="<< lml << "\n\n";
        return lml;
    }


} /* namespace gpmix */