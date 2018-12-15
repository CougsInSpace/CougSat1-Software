package space.cougs.ground.gui.subsystems.modules;

import java.io.File;
import java.util.List;

public class ListFiles {

	public ListFiles() {

	}
	
	public void listFilesForFolder(final File folder, List<File> files) {
		for (final File fileEntry : folder.listFiles()) {
			if (fileEntry.isDirectory()) {
				listFilesForFolder(fileEntry, files);
			} else {

				files.add(fileEntry);
			}
		}
	}
	

}
