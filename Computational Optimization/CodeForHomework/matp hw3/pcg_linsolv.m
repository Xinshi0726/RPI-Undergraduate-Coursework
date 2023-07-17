function [y] = pcg_linsolv(C,r)
L = C';
U = C;
n = size(L, 1);
% Initialize the solution vector
z = zeros(1, n);

% Perform forward substitution
z(1) = r(1)/L(1,1);
for i = 2:n
    z(i) = (r(i)-dot(L(i,:),z))/L(i,i);
end
z = z';
% Initialize the solution vector
y = zeros(1, n);

% Perform backward substitution
y(n) = z(n)/U(n,n);
for i = n-1:-1:1
    y(i) = (z(i)-dot(U(i,:),y))/U(i,i);
end
y = y';
end