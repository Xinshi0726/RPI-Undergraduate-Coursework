function [x, hist_obj] = quadMin_AltMin(A, b, x0, maxit, lb, ub)

% alternating minimization method for solving
% min_x 0.5*x'*A*x - b'*x
% s.t. lb <= x <= ub

x = x0;

% compute the gradient and maintain it
r = A * x - b;

hist_obj = .5 * (x' * (r - b));

n = length(b);

for iter = 1:maxit
    
    % update all coordinates cyclicly
    for i = 1:n 
        
        % update x(i)
        old_x_i = x(i);
        x(i) = (b(i) - A(i, :) * x + A(i, i) * x(i)) / A(i, i);
        x(i) = max(lb(i), min(ub(i), x(i))); % Project x(i) onto the box constraints
        
        % update r vector in an efficient way
        delta_x_i = x(i) - old_x_i;
        r = r + A(:, i) * delta_x_i;

    end
    
    % save objective value after each cycle
    hist_obj = [hist_obj; .5 * (x' * (r - b))];
    
end

end
