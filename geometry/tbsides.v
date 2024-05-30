module Testbench;

  reg signed [7:0] A[1:0][0:0];
  reg signed [7:0] B[1:0][0:0];
  reg signed [7:0] C[1:0][0:0];
  wire signed [7:0] sub[1:0][2:0];
  wire signed [7:0] Q[2:0][1:0];
  wire signed [7:0] temp1;
  wire signed [7:0] temp2;
  wire signed [7:0] temp3;
  wire signed [7:0] side_AB;
  wire signed [7:0] side_BC;
  wire signed [7:0] side_CA;
  
  Matsub mysub (
    .A(A),
    .B(B),
    .C(C),
    .sub(sub)
  );
  
  trans1 Trans1(
    .A(A),
    .B(B),
    .C(C),
    .sub(sub),
    .Q(Q)
  );   
  
  matdot Matdot(
    .A(A),
    .B(B),
    .C(C),
    .sub(sub),
    .Q(Q),
    .temp1(temp1),
    .temp2(temp2),
    .temp3(temp3)
  );
  
  dis Distance(
    .A(A),
    .B(B),
    .C(C),
    .sub(sub),
    .Q(Q),
    .temp1(temp1),
    .temp2(temp2),
    .temp3(temp3),
    .side_AB(side_AB),
    .side_BC(side_BC),
    .side_CA(side_CA)
  );
  

  initial begin

    A[0][0] = 1; 
    A[1][0] = -1; 
    B[0][0] = -4; 
    B[1][0] = 6; 
    C[0][0] = -3; 
    C[1][0] = -5; 

    /*#1;
    $display("Input Values are: \n x1 = %d,\n y1 = %d, \n x2 = %d,\n y2 = %d,\n x3 = %d, \n y3 = %d", A[0][0], A[1][0], B[0][0], B[1][0], C[0][0], C[1][0]);

    #1;
    $display("MatSub value is:");
    #1;
    for (int i = 0; i < 2; i = i + 1) begin
      for (int j = 0; j < 3; j = j + 1) begin
        $write("   %d", sub[i][j]);
      end
      $write("\n");
    end

    $display("Mattran1 value is:");
    #1;
    for (int i = 0; i < 3; i = i + 1) begin
      for (int j = 0; j < 2; j = j + 1) begin
        $write("   %d", Q[i][j]);
      end
      $write("\n");
    end

    $display("Matdot values :");
    #10;
        $write(" matdot A:  %d", temp1);
        $write("\n");
        
        $write(" matdot B:  %d", temp2);
        $write("\n");
 
        $write(" matdot C:  %d", temp3);
        $write("\n");
        */
      $display("sides of a triangle :");
    #10;
     
        $write(" AB :  %f", side_AB);
        $write("\n");
        
        $write(" BC :  %f", side_BC);
        $write("\n");
 
        $write(" CA :  %f", side_CA);
      $write("\n");
 end
endmodule

