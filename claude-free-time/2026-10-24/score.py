import json,re,collections
o=json.load(open('classified.json'))
for r,v in o.items():
    if v['cls']=='C' and v['alpha']>=0.45 and v['ext'][3]>2500: v['cls']='W'  # wanderer: diffusive drift, paves new cells linearly
json.dump(o,open('classified.json','w'),indent=0)
pred=dict(re.findall(r'^\| (R[RL]+) \| (\w)',open('01_predictions.md').read(),re.M))
hit=0;lines=[]
for r,p in pred.items():
    a=o[r]['cls']; ok=p==a; hit+=ok
    lines.append(f"{r:6s} pred {p} actual {a} {'○' if ok else '×'}")
print('\n'.join(lines)); print('individual accuracy',hit,'/',len(pred))
base=sum(o[r]['cls']=='C' for r in pred); print('all-C baseline',base,'/',len(pred))
# P3: among primitive, length>=3, non-S
prim=[r for r,v in o.items() if v['prim']==r and len(r)>=3 and v['cls']!='S']
print('P3 highway fraction (primitive, len>=3, non-S):',sum(o[r]['cls']=='H' for r in prim),'/',len(prim))
c=collections.Counter((len(r),v['cls']) for r,v in o.items() if v['prim']==r)
print('primitive rules by length:')
for L in range(2,9): print(L,{k:c[(L,k)] for k in 'HSGWC'}, 'C share', round(c[(L,'C')]/max(1,sum(c[(L,k)] for k in 'HSGWC')),2))
print('W:',[r for r,v in o.items() if v['cls']=='W'])
print('G:',[r for r,v in o.items() if v['cls']=='G'])
print('slow highways (onset>1e6):',sorted([(v['onset'],r) for r,v in o.items() if v['cls']=='H' and v['onset']>1e6]))
