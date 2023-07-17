function [x, hist_res] = quadMin_gd(A,b,x0,tol)

% conjugate gradient method for solving
% min_x 0.5*x'*A*x - b'*x

% get the size of the problem
n = length(b);

x = x0;

% compute vector r, i.e., gradient of the objective
r = A*x-b;

% set the first p vector as negative gradient
p = -r;

% evaluate the norm of gradient
res = norm(r);

% save the value of res
hist_res = res;

while res > tol
    
    % compute alpha
    
    alpha = res^2/(p'*(A*p));
    
    % update x 
    
    x = x + alpha*p;
    
    % update r
    
    r = A*x-b;
    
    % compute beta
    
    beta = norm(r)^2/(res)^2;
    
    % obtain the new p vector
    
    p = -r+beta*p;
    
    % evaluate the norm of residual vector r
    res = norm(r);
    
    % save the value of res
    hist_res = [hist_res; res];
end

end