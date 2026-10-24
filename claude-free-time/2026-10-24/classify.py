import collections, math, json, re
Ns=[1000000,5000000,20000000,80000000]
d=collections.defaultdict(dict)
for N in Ns:
    for line in open(f'results_N{N}.txt'):
        f=line.split(); d[f[0]][N]=f
def runs_even(s):
    # cyclic runs all even length
    if len(set(s))==1: return False
    i=0
    while s[i]==s[-1]: i+=1  # rotate so run boundary at start
    t=s[i:]+s[:i]
    return all(len(m.group())%2==0 for m in re.finditer(r'R+|L+',t))
def prim(s):
    for p in range(1,len(s)):
        if len(s)%p==0 and s[:p]*(len(s)//p)==s: return s[:p]
    return s
out={}
for r,v in d.items():
    f=v[80000000]; e=[int(v[N][7]) for N in Ns]
    T=int(f[6]); checks=int(f[8]); hits=int(f[9])
    alpha=math.log(e[3]/e[2])/math.log(4)
    if f[1]=='H' and T<80000000: cls='H'
    elif checks and hits/checks>0.5: cls='S'
    elif e[3]<160: cls='G'   # slow, compact growth
    else: cls='C'
    out[r]=dict(cls=cls,onset=int(f[2]) if cls=='H' else None,period=int(f[3]) if cls=='H' else None,
                ext=e,alpha=round(alpha,2),even=runs_even(r),prim=prim(r),returns=checks)
json.dump(out,open('classified.json','w'),indent=0,ensure_ascii=False)
by=collections.Counter((len(r),o['cls']) for r,o in out.items())
for L in range(2,9): print(L,{c:by[(L,c)] for c in 'HSGC'})
print('P2 check (even runs -> S):')
for r,o in sorted(out.items(),key=lambda x:(len(x[0]),x[0])):
    if o['even']!=(o['cls']=='S'): print('  MISMATCH',r,o['cls'],'even' if o['even'] else 'odd')
print('even&S count',sum(o['even'] and o['cls']=='S' for o in out.values()))
print('P1 check:')
for r,o in out.items():
    if o['prim']!=r and (o['cls'],o['onset'],o['ext'])!=(out[o['prim']]['cls'],out[o['prim']]['onset'],out[o['prim']]['ext']) : print('  P1 fail',r,o['prim'])
print('alpha of C:',sorted(o['alpha'] for o in out.values() if o['cls']=='C'))
print('alpha of G:',sorted(o['alpha'] for o in out.values() if o['cls']=='G'))
