clear; close all;

%% generate the data
n = 1000;
e = ones(n,1);
[U,~] = qr(randn(n));
d = linspace(1e-2,1e4,n);
A = U*diag(d)*U';
b = rand(n,1) - 0.5;
x0 = zeros(n,1);
tol = 1e-5;

%% call your solver and show the results

t0 = tic;
[x_s, hist_res_s] = quadMin_pcg(A,triu(A),b,x0,tol);
t1 = toc(t0);

% print results
fprintf('Student solver: Total iteration is %d\n', length(hist_res_s));

fprintf('Total running time is %5.4f\n', t1);

fprintf('Final objective value is %5.4f\n', .5*x_s'*A*x_s - b'*x_s);

fig = figure('papersize',[5,4],'paperposition',[0,0,5,4]);

semilogy(hist_res_s, 'b-','linewidth',2);

xlabel('Iteration number','fontsize',12);
ylabel('Gradient norm','fontsize',12);

%% call instructor's pcg solver

t0 = tic;
[x_p, hist_res_p] = quadMin_pcg_p(A,triu(A),b,x0,tol);
t1 = toc(t0);

% print results

fprintf('Instructor pcg solver: Total iteration is %d\n', length(hist_res_p));

fprintf('Total running time is %5.4f\n', t1);

fprintf('Final objective value is %5.4f\n', .5*x_p'*A*x_p - b'*x_p);

fig = figure('papersize',[5,4],'paperposition',[0,0,5,4]);

semilogy(hist_res_p, 'k-','linewidth',2);

xlabel('Iteration number','fontsize',12);
ylabel('Gradient norm','fontsize',12);


%% call instructor's cg solver

[x_p, hist_res_p] = quadMin_cg_p(A,b,x0,tol);

% print results

fprintf('Instructor cg solver: Total iteration is %d\n', length(hist_res_p));

fprintf('Final objective value is %5.4f\n', .5*x_p'*A*x_p - b'*x_p);

fig = figure('papersize',[5,4],'paperposition',[0,0,5,4]);

semilogy(hist_res_p, 'k-','linewidth',2);

xlabel('Iteration number','fontsize',12);
ylabel('Gradient norm','fontsize',12);
