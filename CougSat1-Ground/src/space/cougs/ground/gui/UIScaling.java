package space.cougs.ground.gui;

interface UIScaling {
	
	enum UIScale{SCALE_75, SCALE_100, SCALE_150, SCALE_200, SCALE_300};
	
	void updateUIScaling(UIScale uiScale);
	
}
