<form action="<? echo $_ENV[SCRIPT_NAME] ?>" method="POST">
  <input type="hidden" name="name" value="<? echo $admin ?>">
  <center>
    <table>
      <tr>
        <td>User</td><td>:</td><td><input name="user" value="<? echo $user ?>" size="16"></td>
      </tr>
      <tr>
        <td>New password</td><td>:</td>
        <td><input name="newpass1" size="16" type="password"></td>
      </tr>
      <tr>
        <td>Re-enter new password</td><td>:</td>
        <td><input name="newpass2" size="16\" type="password"></td>
      </tr>
    </table>
    <input type="reset" value="Clear">
    <input type="submit" value="Update  Password">
  </center>
</form>
