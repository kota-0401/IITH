package net.codejava;

import java.sql.*;
import java.util.Scanner;

public class ex4 {
	public static void main(String[] args) {
	    Scanner sc = new Scanner(System.in);
	    System.out.print("Enter roll number: ");
	    String roll = sc.nextLine();

	    Connection conn = null;
	    Statement stmt = null;
    
    try {
      
      Class.forName("org.postgresql.Driver");
      conn = DriverManager.getConnection(
          "jdbc:postgresql://localhost:5432/univdb", "postgres", "Vignankota");
      
   
      stmt = conn.createStatement();
      stmt.executeUpdate("ALTER TABLE takes ADD COLUMN gpa INT");
      
      
      stmt.executeUpdate("UPDATE takes SET gpa = 10 WHERE grade = 'A+'");
      stmt.executeUpdate("UPDATE takes SET gpa = 9 WHERE grade = 'A'");
      stmt.executeUpdate("UPDATE takes SET gpa = 8 WHERE grade = 'A-'");
      stmt.executeUpdate("UPDATE takes SET gpa = 7 WHERE grade = 'B+'");
      stmt.executeUpdate("UPDATE takes SET gpa = 6 WHERE grade = 'B'");
      stmt.executeUpdate("UPDATE takes SET gpa = 5 WHERE grade = 'B-'");
      stmt.executeUpdate("UPDATE takes SET gpa = 4 WHERE grade = 'C+'");
      stmt.executeUpdate("UPDATE takes SET gpa = 3 WHERE grade = 'C'");
      stmt.executeUpdate("UPDATE takes SET gpa = 2 WHERE grade = 'C-'");
      
      
      String sql = "SELECT AVG(gpa)AS CGPA FROM takes WHERE id = '" + roll + "';";
      ResultSet rs = stmt.executeQuery(sql);
      if (rs.next()) {
          float cgpa = rs.getFloat("CGPA");
          System.out.println("CGPA: " + cgpa);
        } else {
          System.out.println("Error: roll number not found.");
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
