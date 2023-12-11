#Q = {0,1,2,3,4,accept,reject,clean,reject1,reject2,reject3,reject4,reject5,reject6,reject7,reject8,reject9,reject10,reject11,reject12,reject13}
#S = {a,b}
#G = {a,b,c,_,X,I,l,e,g,n,p,u,t}
#q0 = 0
#B = _
#F = {accept}
#N = 2

0 a* _* r* 1
0 b* ** ** reject
0 _* ** ** reject

1 a* ** r* 1
1 b* *X rr 2
1 _* ** ** reject

2 a* ** ** reject
2 b* *X rr 2
2 _* ** l* 3

3 a* ** l* 4
3 b* ** l* 3
3 _* ** rl accept

4 a* ** l* 4
4 _* ** r* 0

accept *X c_ rl accept

reject a* ** r* reject
reject b* ** r* reject
reject _* ** l* clean

clean a* _* l* clean
clean b* _* l* clean
clean _* ** ** reject0

reject0 ** I* r* reject1
reject1 ** l* r* reject2
reject2 ** l* r* reject3
reject3 ** e* r* reject4
reject4 ** g* r* reject5
reject5 ** a* r* reject6
reject6 ** l* r* reject7
reject7 ** _* r* reject8
reject8 ** I* r* reject9
reject9 ** n* r* reject10
reject10 ** p* r* reject11
reject11 ** u* r* reject12
reject12 ** t* r* reject13