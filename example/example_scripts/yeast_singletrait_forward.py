import limix.modules.qtl as QTL
import scipy as SP
import pandas as pd

#create a complex query on the gene_ID and environment:
# select environment 0 for gene YBR115C
phenotype_query = "(gene_ID=='YBR115C') & (environment==0)"

data_subsample = data.subsample_phenotypes(phenotype_query=phenotype_query,intersection=True)

#get variables we need from data
snps = data_subsample.getGenotypes(impute_missing=True)
phenotypes,sample_idx = data_subsample.getPhenotypes(phenotype_query=phenotype_query,intersection=True); assert sample_idx.all()

sample_relatedness = data_subsample.getCovariance()
pos = data_subsample.getPos()

#set parameters for the analysis
N, P = phenotypes.shape          

covs = None                 #covariates
searchDelta = False         #specify if delta should be optimized for each SNP
test="lrt"                  #specify type of statistical test

# Running the analysis
threshold = 0.01/snps.shape[1] # inclusion threshold on pvalues (Bonferroni corrected significance level of 1%)
maxiter = 10                # maximum number of iterations
lmm, RV = QTL.forward_lmm(snps,phenotypes.values,K=sample_relatedness,qvalues=False,threshold=threshold,maxiter=maxiter)
# RV contains:
# - iadded:   array of indices of SNPs included in order of inclusion
# - pvadded:  array of Pvalues obtained by the included SNPs in iteration before inclusion
# - pvall:   [maxiter x S] SP.array of Pvalues for all iterations

#convert P-values to a DataFrame for nice output writing:
iteration = SP.arange(0,len(RV['pvadded'])+1)
pvalues = pd.DataFrame(data=RV['pvall'][:len(iteration)].T,index=data_subsample.geno_ID,columns=iteration)
pvalues = pd.concat([pos,pvalues],join="inner",axis=1)

pvalues_added = pd.DataFrame(data=RV['pvadded'],index=data_subsample.geno_ID[RV['iadded']],columns=["P-value_inclusion"])
pvalues_added = pd.concat([pos,pvalues_added],join="inner",axis=1)

#create result DataFrame
result["pvalues_all"] = pvalues
result["pvalues_added"] = pvalues_added