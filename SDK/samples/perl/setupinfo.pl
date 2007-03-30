use Win32::OLE;
use Win32::OLE::Variant;
my $session = Win32::OLE->new('MiKTeX.Session')
  or die "MiKTeX is not properly installed.\n";
my $path = Variant(VT_BSTR|VT_BYREF, '');
my $setupInfo = $session->GetMiKTeXSetupInfo();
print "MiKTeX: $setupInfo->{'version'}\n";
if ($setupInfo->{'sharedSetup'}) {
  print "SharedSetup: yes\n";
} else {
  print "SharedSetup: no\n";
}
print "BinDir: $setupInfo->{'binDirectory'}\n";
for ($r = 0; $r < $setupInfo->{'numRoots'}; $r += 1) {
  my $rootDirectory = $session->GetRootDirectory($r);
  print "Root$r: $rootDirectory\n";
}
print "Install: $setupInfo->{'installRoot'}\n";
print "UserData: $setupInfo->{'userDataRoot'}\n";
print "UserConfig: $setupInfo->{'userConfigRoot'}\n";
if ($setupInfo->{'sharedSetup'}) {
  print "CommonData: $setupInfo->{'commonDataRoot'}\n";
  print "CommonConfig: $setupInfo->{'commonConfigRoot'}\n";
}
