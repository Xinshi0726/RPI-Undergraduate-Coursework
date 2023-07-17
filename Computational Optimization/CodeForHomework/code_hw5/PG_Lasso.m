function [x,hist_res] = PG_Lasso(A,b,x0,lam,tol)

% proximal gradient method for solving the Lasso
% min_x .5*||A*x-b||^2 + lam*||x||_1

% compute Lipschitz constant
L = norm(A*A');

% compute gradient at x0
grad = A'*(A*x0-b);

% perform one proximal gradient step with stepsize 1/L to get a new x
x = sign(x0 - grad/L) .* max(abs(x0 - grad/L) - lam/L, 0);

% evaluate norm of the proximal gradient mapping
res = L * norm(x-x0);

hist_res = res;

while res > tol
    x0 = x;

    % compute gradient at x0
    grad = A'*(A*x0-b);

    % perform one proximal gradient step with stepsize 1/L to get a new x
    x = sign(x0 - grad/L) .* max(abs(x0 - grad/L) - lam/L, 0);

    % evaluate norm of the proximal gradient mapping
    res = L * norm(x-x0);
    hist_res = [hist_res; res];
end

end

% define the proximal operator for the L1 norm
function y = prox_l1(x, t)
    y = sign(x) .* max(abs(x) - t, 0);
end
