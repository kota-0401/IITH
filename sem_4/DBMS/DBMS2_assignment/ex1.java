package net.codejava;

import java.util.*;
import java.sql.*;

public class ex1 {
	
   static void printTable(String tableName, int k) {
    try (Connection conn = DriverManager.getConnection("jdbc:postgresql://localhost:5432/univdb",
            "postgres", "Vignankota")) {
      DatabaseMetaData meta = conn.getMetaData();
      String sql = "SELECT * FROM " + tableName + " LIMIT " + k;
      Statement stmt = conn.createStatement();
      ResultSet res = stmt.executeQuery(sql);
      System.out.println("Table: " + tableName);

     
      int numOfColumns = res.getMetaData().getColumnCount();
      for (int i = 1; i <= numOfColumns; i++) {
        System.out.print(res.getMetaData().getColumnName(i) + "\t");
      }
      System.out.println();

   
      while (res.next()) {
        for (int i = 1; i <= numOfColumns; i++) {
          System.out.print(res.getString(i) + "\t");
        }
        System.out.println();
      }
    } catch (SQLException e) {
      e.printStackTrace();
    }
  }
	
  public static void main(String[] args) {
	  Scanner sc = new Scanner(System.in);   
	  System.out.print("Enter the table: ");  
	  String str = sc.nextLine();              
	  System.out.print("Enter the row limit: ");  
	  int k = sc.nextInt();
	  printTable(str,k);
  }
}
