package net.codejava;

import java.sql.*;
import java.util.Scanner;

public class ex5c {
	public static void main(String[] args) {
	    Scanner sc = new Scanner(System.in);
	    System.out.print("Enter the course_id: ");  
		String course_id = sc.nextLine();
	    System.out.print("Enter the row limit: ");  
		int k = sc.nextInt();

	    Connection conn = null;
	    Statement stmt = null;
    
    try {
      
      Class.forName("org.postgresql.Driver");
      conn = DriverManager.getConnection(
          "jdbc:postgresql://localhost:5432/univdb", "postgres", "Vignankota");
      
   
      stmt = conn.createStatement();
      
      String sql = "SELECT name, cgpa FROM student NATURAL JOIN takes WHERE takes.course_id = '" + course_id + "' ORDER BY cgpa DESC LIMIT " + k;
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