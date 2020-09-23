# matrix-cpp

<h3>Small implementaion of Matrices in C++ to simplify process of working with them.</h3>
<div>
  To use this class, add it directly into your project (or create library).
  <br/>
  Tested on Windows 10.
</div>

<h4>Example</h4>
<pre>
<code>
Matrix A = Matrix("matrixA.txt");
Matrix B = Matrix("matrixB.txt");
std::cout << "==============" << std::endl;
Matrix C = A + B; // A * B ...
C.print();
</code>
</pre>

<div>
  <h4>Supports:</h4>
  <table>
    <tr>
      <td>addition</td>
      <td><b>A + B</b></td>
    </tr>
    <tr>
      <td>subtraction</td>
      <td><b>A - B</b></td>
    </tr>
    <tr>
      <td>multiplication</td>
      <td><b>A * B / scalar * A</b></td>
    </tr>
    <tr>
      <td>transpose</td>
      <td><b>~A</b></td>
    </tr>
    <tr>
      <td>inverse (if there is one)</td>
      <td><b>A.inverse()</b></td>
    </tr>
  </table>
</div>
