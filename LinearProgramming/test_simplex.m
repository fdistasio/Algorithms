A1 = [1,2;-1,2;-1,-2;1,-2;0,-1];
b1 = [2;2;2;2;1];
B1 = [1,2];
c1 = [0,2];

A2 = [0,-1;-1,1;-2,3;0,2;-3,3];
b2 = [0;1;3;2;6];
B2 = [3,4];
c2 = [1,2];

A3 = [-1,-1;-2,-1;0,-1;1,0;0,1];
b3 = [-3;-7;-1;2;2];
B3 = [4,5];
c3 = [4,2];

[B1, x1, y1, status1] = primal_simplex(A1, b1, c1, B1); % optimal
[B2, x2, y2, status2] = primal_simplex(A2, b2, c2, B2); % Primal unbounded
[B3, x3, y3, status3] = dual_simplex(A3, b3, c3,B3);    % Primal empty