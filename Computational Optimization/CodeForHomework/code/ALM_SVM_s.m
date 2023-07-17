function [w,b,out] = ALM_SVM(X,y,lam,opts)
%=============================================
%
% augmented Lagrangian method for solving SVM
% min_{w,b,t} sum(t) + lam/2*norm(w)^2
% s.t. y(i)*(w'*X(:,i)+b) >= 1-t(i)
%      t(i) >= 0, i = 1,...,N
%
%===============================================
%
% ==============================================
% input:
%       X: training data, each column is a sample data
%       y: label vector
%       lam: model parameter
%       opts.tol: stopping tolerance
%       opts.maxit: maximum number of outer iteration
%       opts.subtol: stopping tolerance for inner-loop
%       opts.maxsubit: maxinum number of iteration for inner-loop
%       opts.w0: initial w
%       opts.b0: initial b0
%       opts.t0: initial t0
%       opts.beta: penalty parameter
%
% output:
%       w: learned w
%       b: learned b
%       out.hist_pres: historical primal residual
%       out.hist_dres: historical dual residual
%       out.hist_subit: historical iteration number of inner-loop

% ======================================================

%% get size of problem: p is dimension; N is number of data pts
X = X';
[N,p] = size(X);

%% set parameters
if isfield(opts,'tol')        tol = opts.tol;           else tol = 1e-4;       end
if isfield(opts,'maxit')      maxit = opts.maxit;       else maxit = 500;      end
if isfield(opts,'subtol')     subtol = opts.subtol;     else subtol = 1e-4;    end
if isfield(opts,'maxsubit')   maxsubit = opts.maxsubit; else maxsubit = 5000;  end
if isfield(opts,'w0')         w = opts.w0;              else w = randn(p,1);   end
if isfield(opts,'b0')         b = opts.b0;              else b = 0;            end
if isfield(opts,'t0')         t = opts.t0;              else t = zeros(N,1);   end
if isfield(opts,'beta')       beta = opts.beta;         else beta = 1;  end

%% add the main part by yourself
%% initialize variables
u = randn(N, 1);
hist_pres = zeros(maxit, 1);
hist_dres = zeros(maxit, 1);
hist_subit = zeros(maxit, 1);

%% main loop
for k = 1:maxit
    % inner loop to minimize augmented Lagrangian with respect to primal variables
    for subit = 1:maxsubit
        w_old = w;
        b_old = b;
        t_old = t;
        eta = 0.001;
        u_old = u;
        % update w
        grad_w = lam*w + beta*sum(max(0,1-t-y.*(X*w+b)+u/beta).*-y.*X)';
        w = w - eta * grad_w;
        
        % update b
        grad_b = beta*(sum(max(0,1-t-y.*(X*w+b)+u/beta).*-y));
        b = b - eta * grad_b;
        % max(0,1-t.*(X*w+b)+u/beta).*-y-y
        % update t
        grad_t = 1-beta * max(0,1-t-y.*(X*w+b)+u/beta);
        t = t - eta * grad_t;
        t = max(0,t);
        % check for convergence
        if max([norm(w - w_old), norm(b - b_old), norm(t - t_old)])/eta < subtol
            break;
        end
    end
    % update u
    u = max(0,u + beta *(1 - t - y .* (X * w + b)));
 
    % compute primal and dual residuals
    pres = beta*norm(max(0,1-t-y.*(X*w+b)));
    dres = beta*norm(u-u_old);
    % store historical residuals and inner-loop iterations
    hist_pres(k+1) = pres;
    hist_dres(k+1) = dres;
    hist_subit(k+1) = subit;

    % check convergence
    if max(pres, dres) < tol
        break;
    end
end
%% return pres, dres, and subit
out.hist_pres = hist_pres;
out.hist_dres = hist_dres;
out.hist_subit = hist_subit;


end



