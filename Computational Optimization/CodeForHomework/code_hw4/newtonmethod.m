x = [-1.5 1.5];
iter = 0;
%% main iteration
while iter < 10
    grad = [-200*x(1)*x(2)+200*(x(1)^3)-2+2*x(1) 100*x(2)-100*(x(1)^2)];
    hessian = [-200*x(2)+600*(x(1)^2)+2 -200*x(1);-200*x(1) 100];
    x = x-inv(hessian)*grad';
    fprintf('Iteration %d: Norm of grad is %f\n',iter+1, norm(grad));
    iter = iter+1;
end
fprintf('The final minimizer is x1 = %f and x2 = %f',x(1),x(2));

