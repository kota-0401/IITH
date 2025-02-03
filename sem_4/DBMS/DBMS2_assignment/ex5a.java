package net.codejava;

import java.sql.*;
import java.util.Scanner;

public class ex5a {
	public static void main(String[] args) {
	    Scanner sc = new Scanner(System.in);
	    
	    System.out.print("Enter the row limit: ");  
		int k = sc.nextInt();

	    Connection conn = null;
	    Statement stmt = null;
    
    try {
      
      Class.forName("org.postgresql.Driver");
      conn = DriverManager.getConnection(
          "jdbc:postgresql://localhost:5432/univdb", "postgres", "Vignankota");
      
   
      stmt = conn.createStatement();
      stmt.executeUpdate("ALTER TABLE student ADD COLUMN cgpa FLOAT(6)");
      
      
      stmt.executeUpdate("UPDATE student SET cgpa = (SELECT AVG(gpa) FROM takes WHERE student.id = takes.id)");
      
      
      
      String sql = "SELECT name, cgpa FROM student" + " ORDER BY cgpa DESC LIMIT " + k;
      ResultSet rs = stmt.executeQuery(sql);
      while (rs.next()) {
          String name = rs.getString("name");
          double cgpa = rs.getDouble("cgpa");

          System.out.println(name + ": " + cgpa);
        }

        rs.close();
        stmt.close();
        conn.close();
      } catch (SQLException se) {
        se.printStackTrace();
      } catch (Exception e) {
        e.printStackTrace();
      } finally {
        try {
          if (stmt != null) stmt.close();
        } catch (SQLException se2) {
        }
        try {
          if (conn != null) conn.close();
        } catch (SQLException se) {
          se.printStackTrace();
        }
      }
    }
  }
