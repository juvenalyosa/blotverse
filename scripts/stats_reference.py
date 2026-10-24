#!/usr/bin/env python3
"""Reference values for tests/test_stats.cpp (SciPy and statsmodels MixedLM, REML).

Run in an environment with scipy, statsmodels and pandas:
    python3 scripts/stats_reference.py
"""
import numpy as np, scipy.stats as st, statsmodels.formula.api as smf, pandas as pd, warnings
warnings.filterwarnings("ignore")
a=[1.1,0.9,1.3,1.05,0.95]; b=[1.6,1.9,1.4,2.2,1.75,1.8]
r=st.ttest_ind(b,a,equal_var=False); print("welch t=%.15g p=%.15g"%(r.statistic,r.pvalue))
print("tp", "%.15g"%(2*st.t.sf(2.1,7.3)), "%.15g"%(2*st.t.sf(0.5,3)), "%.15g"%(2*st.t.sf(10,30)))
print("ibeta %.15g %.15g"%(st.beta.cdf(0.3,2.5,1.5), st.beta.cdf(0.9,0.5,7)))
# mixed model: 3 gels, 2 ctrl + 2 treat per gel
y=[0.1,0.3,1.2,0.9, 0.6,0.5,1.4,1.7, -0.2,0.0,0.8,0.7]
t=[0,0,1,1]*3; g=[0]*4+[1]*4+[2]*4
d=pd.DataFrame(dict(y=y,t=t,g=g))
m=smf.mixedlm("y~t",d,groups=d["g"]).fit(reml=True)
print("mixed b1=%.12g se=%.12g b0=%.12g vc=%.12g scale=%.12g"%(m.params["t"],m.bse["t"],m.params["Intercept"],m.cov_re.iloc[0,0],m.scale))
