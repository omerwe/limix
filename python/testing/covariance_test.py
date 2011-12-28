import sys
sys.path.append('./..')
sys.path.append('./../../../pygp')


import gpmix
import pygp.covar.linear as lin
import pygp.covar.se as se
import pygp.covar.gradcheck as GC
import scipy as SP
import pdb

n_dimensions=3
X = SP.randn(3,n_dimensions)

params = SP.zeros([0])

if 0:
    c1 = gpmix.CCovLinearISO()
    c2 = lin.LinearCFISO(n_dimensions=n_dimensions)

    c1.setX(X)

    K1 = c1.K()
    K2 = c2.K(params,X,X)

    dK1 = c1.Kgrad_param(0)
    dK2 = c2.Kgrad_theta(params,X,0)

    dKx1= c1.Kgrad_X(0)
    dKx2= c2.Kgrad_x(params,X,X,0)

    dKx1diag = c1.Kdiag_grad_X(0)
    dKx2diag = c2.Kgrad_xdiag(params,X,0)


c1 = gpmix.CCovSqexpARD()
c2 = se.SqexpCFARD(n_dimensions=n_dimensions)

params = SP.random.randn(n_dimensions+1)
#params[:] = 0
c1.setX(X)
c1.setParams(params)

K1 = c1.K()
K2 = c2.K(params,X,X)

print SP.absolute(K1-K2).max()

dK1 = c1.Kgrad_param(0)
dK2 = c2.Kgrad_theta(params,X,0)

dKx1= c1.Kgrad_X(0)
dKx2= c2.Kgrad_x(params,X,X,0)

print SP.absolute(dK1-dK2).max()

