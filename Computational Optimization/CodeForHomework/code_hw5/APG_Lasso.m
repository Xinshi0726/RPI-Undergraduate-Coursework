function [x,hist_res] = APG_Lasso(A,b,x0,lam,tol)

% accelerated proximal gradient method for solving the Lasso
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

x0 = x;

% y used to denote the extrapolated point
y = x;

% used to compute the extrapolation weight
t0 = 1;

while res > tol
    
    % compute a gradient at y
    grad = A'*(A*y-b);
    
    % perform one proximal gradient step with stepsize 1/L to get a new x
    x_new = sign(y - grad/L) .* max(abs(y - grad/L) - lam/L, 0);
    
    % update t value to compute extrapolation weight
    t1 = (1 + sqrt(1 + 4*t0^2))/2;
    
    % update the extrapolated point y
    y = x_new + (t0 - 1)/t1 * (x_new - x);
    
    x = x_new;
    
    % evaluate norm of the proximal gradient mapping
    res = L * norm(x-x0);
    hist_res = [hist_res; res];
    
    t0 = t1; x0 = x;
end

end


