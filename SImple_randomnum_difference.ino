void setup() {
 
  Serial.begin(9600); // put your setup code here, to run once:

}

void loop() {
  int p = random(100);
  int q = random(100);
  int r = random(100);
  int s = random(100);
  int k;

  Serial.print("p is ");
  Serial.print(p);
  Serial.print(", q is ");
  Serial.print(q);
  Serial.print(", r is ");
  Serial.print(r);
  Serial.print(", s is ");
  Serial.println(s);
  delay(100);
  Serial.print("abs diff of q and r is ");

  k = absoluteDifference (q, r);
  Serial.println(k);
  if (k > p) {
  Serial.println("abs diff p and r > q is true");
}
if (k < p) {
  Serial.println("abs diff p and r < q is flase");
}
Serial.print("abs diff of abs diff p and q and abs diff r and s is ");
Serial.println (absoluteDifference(absoluteDifference(p,q),absoluteDifference(r,s)));

Serial.println("");
  delay(500);
}
  int absoluteDifference(int x, int y){
  int result;
  result = abs(x - y);
   return result;
  }
