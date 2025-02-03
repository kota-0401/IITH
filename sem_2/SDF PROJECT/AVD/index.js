const mysql = require('mysql');
const express = require ('express');
const cors = require('cors');
const bodyParser = require('body-parser');
const app = express();
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
  extended: true
}));
app.use(cors());
app.use(express.json());
app.listen(3001, () => console.log('Server running at port 3001'));

const encoder = bodyParser.urlencoded();
const db = mysql.createConnection({
   host: 'localhost',
   user: 'ubuntu',
   password: '321321',
   database: 'avd'
});

// app.get('/signup',(req,res)=>{
//   db.query('select * from SignUp',(err,rows,fields)=>{
//     if(!err)
//     res.send(rows);
//     else console.log(err);
//   })
// });
// db.connect((err) =>{
//    if (err) throw err;
//    console.log('mysql Connected');
// });

db.connect(function(err) {
   if (err) throw err;
   console.log("Connected!");
   var sql = "INSERT INTO SignUp (CustomerName,CustomerEmail,CustomerPassword) VALUES ('abcd','abcd@gmail.com','abcd25')";
   db.query(sql, function (err, result) {
     if (err) throw err;
     console.log("1 record inserted");
   });
 });
// connection.query("INSERT INTO SignUp (CustomerName,CustomerEmail,CustomerPassword) VALUES ?",[cname,email,password],function(error,results,fields){
//   if(err) throw err;
// console.log("record inserted")
// });

app.get("/",function(req,res){
  res.sendfile(__dirname + "/login.html");
 })

 app.post("/",encoder,function(req,res){
  var email = req.body.email;
  var password  = req.body.password;

  connection.query("select * from SignUp where CustomerEmail= ? and CustomerPassword = ?",[email,password],function(error,results,fields){
    if(results.length>0){
      res.redirect("customerpanel");
    } else{
      res.redirect("/");
    }
    res.end();
  })
 })
app.get("/customerpanel",function(req,res){
  res.sendfile(__dirname+"/customerpanel.html")
})

