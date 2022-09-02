import { initializeApp } from "https://www.gstatic.com/firebasejs/9.8.4/firebase-app.js";
import { getAnalytics } from "https://www.gstatic.com/firebasejs/9.8.4/firebase-analytics.js";

// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyAIBdqdTdM10eTcVfuPGzZSk56RMsvk68E",
  authDomain: "bees-central.firebaseapp.com",
  databaseURL: "https://bees-central-default-rtdb.firebaseio.com",
  projectId: "bees-central",
  storageBucket: "bees-central.appspot.com",
  messagingSenderId: "291740837080",
  appId: "1:291740837080:web:2725d68da2ed541c7d63ad",
  measurementId: "G-H7XQMQ14Z3"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const analytics = getAnalytics(app);