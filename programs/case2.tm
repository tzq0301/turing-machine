#Q = {0,lempty,1,2,accept,accept0,accept1,accept2,accept3,reject,rejectclean,reject0,reject1,reject2,reject3,reject4}
#S = {a,b,c}
#G = {a,b,c,_,t,r,u,e,f,a,l,s}
#q0 = 0
#B = _
#F = {accept}
#N = 2

0 a* *a rr 0
0 b* *b rr 0
0 c* ** rl lempty
0 _* ** ** reject

lempty a* ** ** 1
lempty b* ** ** 1
lempty _* ** ** reject

1 *a ** *l 1
1 *b ** *l 1
1 *_ ** *r 2
1 _* ** ** reject

2 aa ** rr 2
2 bb ** rr 2
2 _* ** l* accept
2 ** ** ** reject

accept a* _* l* accept
accept b* _* l* accept
accept c* _* l* accept

accept _* t* r* accept0
accept0 _* r* r* accept1
accept1 _* u* r* accept2
accept2 _* e* r* accept3

reject a* ** r* reject
reject b* ** r* reject
reject c* ** r* reject
reject _* ** l* rejectclean

rejectclean a* _* l* rejectclean
rejectclean b* _* l* rejectclean
rejectclean c* _* l* rejectclean
rejectclean _* f* r* reject0

reject0 _* a* r* reject1
reject1 _* l* r* reject2
reject2 _* s* r* reject3
reject3 _* e* r* reject4
